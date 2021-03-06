
#include <iostream>
#include <cfloat> // DBL_EPSILON
#include <cassert>
#include <cmath>
#include <sys/time.h> // gettimeofday()
#include <cstring> // memset()
#include <unistd.h> // getpid()

#include <StdCout.hpp>
#include <Memory.hpp>


#ifdef RAND_DSFMT
#include "dSFMT/dSFMT.hpp"
#endif // #ifdef RAND_DSFMT


#include "PseudoRandomNumberGenerator.hpp"

// Special code since "is_initialized" might initially be undefined
const int PRNG_is_initialized       = 12345;
const int PRNG_is_NOT_initialized   = 0;


// **************************************************************
PRNG::PRNG()
{
    is_initialized = PRNG_is_NOT_initialized;
    nb_calls = 0;
#ifdef RAND_DSFMT
    dsfmt_data = NULL;
#endif // #ifdef RAND_DSFMT
}

// **************************************************************
PRNG::~PRNG()
{
#ifdef RAND_DSFMT
    delete ((dsfmt_t *) dsfmt_data);
    dsfmt_data = NULL;
#endif // #ifdef RAND_DSFMT
}

// **************************************************************
void PRNG::Initialize_Taking_Time_As_Seed(const bool quiet)
{
    // Get high precision time
    timeval tv;
    gettimeofday(&tv, 0);
    // typeof(tv.tv_sec)  = time_t (8 bytes unsigned integer)
    // typeof(tv.tv_suec) = suseconds_t (8 bytes unsigned integer)
    // typeof(getpid())   = pid_t (4 bytes unsigned integer)
    uint64_t seconds_since_epoch        = uint64_t(tv.tv_sec);
    uint64_t microseconds_since_seconds = uint64_t(tv.tv_usec);
    uint64_t pid                        = uint64_t(getpid());

    // Set epoch to 2000 instead of 1970. Should give a bigger seed range
    const uint64_t new_epoch_2000                   = uint64_t(60)*uint64_t(60)
                                                        *uint64_t(24)*uint64_t(365)
                                                        *uint64_t(30); // ~30 years
    const uint64_t seconds_since_new_epoch          = seconds_since_epoch - new_epoch_2000;

    // Get number of microseconds since the new epoch
    const uint64_t microseconds_since_new_epoch = (uint64_t(1e6) * seconds_since_new_epoch)
                                                    + microseconds_since_seconds;

    // Multiply by pid to get the seed
    const uint32_t MAX_uint32_t = ~uint32_t(0);
    uint32_t random_number_seed = uint32_t((pid * microseconds_since_new_epoch) % uint64_t(MAX_uint32_t));

    Initialize(random_number_seed, quiet);
}

// **************************************************************
void PRNG::Initialize(const uint32_t new_seed, const bool quiet)
{
#ifdef RAND_DSFMT
    dsfmt_data = new dsfmt_t;
    memset(dsfmt_data, 0, sizeof(dsfmt_t));
#endif // #ifdef RAND_DSFMT

    seed           = new_seed;
    is_initialized = PRNG_is_initialized;
    nb_calls       = 0;
#ifdef RAND_DSFMT
    if (!quiet)
    {
        std_cout
            << "Using SIMD-oriented Fast Mersenne Twister (SFMT)\n"
            << "pseudo-random number generator (PRNG)\n"
            << "See http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/#dSFMT\n";
    }

    dsfmt_init_gen_rand((dsfmt_t *) dsfmt_data, new_seed);
#else  // #ifdef RAND_DSFMT

    std_cout
        << "###################################################################\n"
        << "WARNING!!!! Using plain (crappy) C/C++ rand() function!!!\n"
        << "Please recompile with -DRAND_DSFMT (see Makefile) for a better\n"
        << "pseudo-random number generator.\n"
        << "Enter to continue, Ctrl+C to cancel\n";

    getchar();
    srand(seed);
#endif // #ifdef RAND_DSFMT
    if (!quiet)
    {
        std_cout << "Library's PRNG's seed: " << seed << std::endl;
    }
}

// **************************************************************
double PRNG::Get_Random()
/**
 * Return a pseudo-random number in the interval ]0,1]
 * (excluding 0, including 1)
 */
{
    assert(is_initialized == PRNG_is_initialized);
#ifdef RAND_DSFMT
    assert(dsfmt_data != NULL);
#endif // #ifdef RAND_DSFMT

    return Get_Random_Open0_Close1();
}

// **************************************************************
double PRNG::Get_Random_Close1_Open2()
/**
 * Return a pseudo-random number in the interval [1,2[
 * (including 1, excluding 2).
 * This is the most fondamental function, others are based on
 * this one.
 */
{
    nb_calls++;
#ifdef RAND_DSFMT
    // Use  SIMD-oriented Fast Mersenne Twister (SFMT)
    // See http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/
    // dsfmt_genrand_close1_open2() returns a double between: [1,2[
    return dsfmt_genrand_close1_open2((dsfmt_t *) dsfmt_data);
#else  // #ifdef RAND_DSFMT
    //return double(rand()) / double(RAND_MAX);         // [0,1]
    //return double(rand()) / (double(RAND_MAX) - 1.0); // [0,1[
    return 1.0 + (double(rand()) / (double(RAND_MAX) - 1.0)); // [1,2[
    // The "- 1.0" or "1.0 -" are doubles since RAND_MAX+1 will overflow.
#endif // #ifdef RAND_DSFMT
}

// **************************************************************
double PRNG::Get_Random_Close0_Open1()
/**
 * Return a pseudo-random number in the interval [0,1[
 * (including 0, excluding 1)
 */
{
    // We remove one from [1,2[ to get [0,1[
    return Get_Random_Close1_Open2() - 1.0;
}

// **************************************************************
double PRNG::Get_Random_Open0_Close1()
/**
 * Return a pseudo-random number in the interval ]0,1]
 * (excluding 0, including 1)
 */
{
    // We remove [1,2[ from 2.0 to get something between: ]0,1]
    return (2.0 - Get_Random_Close1_Open2());
}

// **************************************************************
double PRNG::Get_Random_CloseN1_Open1()
/**
 * Return a pseudo-random number in the interval [-1,1[
 * (including -1, excluding 1)
 */
{
    // We remove 3 from 2*[1,2[ = [2,4[ to get [-1,1[
    return ((2.0 * Get_Random_Close1_Open2()) - 3.0);
}

// **************************************************************
double PRNG::Get_Random_Close0_Close1()
/**
 * Return a pseudo-random number in the interval [0,1]
 * (including 0, including 1)
 */
{
    // Get a random number in [1,2[ but only keep it if it's
    // less or equal then 1, giving [0,1]
    double r;
    do {
        r = 2.0*Get_Random_Close0_Open1();
    } while (r > 1.0);
    return r;
}

// **************************************************************
double PRNG::Get_Random_CloseN1_Close1()
/**
 * Return a pseudo-random number in the interval [-1,1]
 * (including -1, including 1)
 */
{
    // We remove 1 from 2*[0,1] = [0,2] to get [-1,1]
    return (2.0*Get_Random_Close0_Close1() - 1.0);
}

// **************************************************************
void PRNG::Get_Random_Direction(double xyz[3])
/**
 * Marsaglia (3D) to uniformly generate directions in three dimension.
 * Reference:
 * Computational physics: an introduction. Franz Vesely
 * 2nd ed., 2001, 278 pages.
 * ISBN: 978-0-306-46631-1
 * See Section 3.2 - "Other distributions", page 67.
 * Online:
 * http://books.google.ca/books?id=_uvEBDtaZvQC&pg=PA67&lpg=PA67&dq=Marsaglia+3d&source=bl&ots=37GTsqWEf-&sig=pRmwuWN4-i0SJkIEX9TMezM8syQ&hl=en&ei=lgxaTOLyNYuesQP3v4HwDw&sa=X&oi=book_result&ct=result&resnum=2&ved=0CBkQ6AEwAQ#v=onepage&q=Marsaglia%203d&f=false
 * http://mathworld.wolfram.com/SpherePointPicking.html
 */
{
    double y1, y2, r2;

    do
    {
        // Pick two uniform numbers in the square [-1,1]
        y1 = Get_Random_CloseN1_Close1();
        y2 = Get_Random_CloseN1_Close1();
        r2 = y1*y1 + y2*y2;
    } while (r2 >= 1.0);

    xyz[0] = 2.0 * y1 * std::sqrt(1.0 - r2);
    xyz[1] = 2.0 * y2 * std::sqrt(1.0 - r2);
    xyz[2] = 1.0 - 2.0*r2;

#ifdef YDEBUG
    const double r = std::sqrt(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    assert(0.9999 < r && r < 1.0001);
#endif // #ifdef YDEBUG
}

// **************************************************************
std::vector<double> PRNG::Get_Random_Direction()
{
    double random_direction[3];
    Get_Random_Direction(random_direction);
    return std::vector<double>(random_direction, random_direction+3);
}

// **************************************************************
double PRNG::gasdev()
/**
 * Returns a normally distributed deviate with zero mean and unit variance.
 * Same as gasdev() from "Numerical Recipes in C", Chapter 7.2, page 289
 * http://en.wikipedia.org/wiki/Box_muller#Polar_form
 */
{
    return Get_Random_Box_Muller_Polar(0.0, 1.0);
}

// **************************************************************
double PRNG::Get_Random_Box_Muller_Polar(const double mean, const double std_dev)
/**
 * Marsaglia's Polar version of Box-Muller to get randoms
 * numbers from a standard normally distribution.
 * The original algorithm has a zero mean and unit variance.
 * It has been adapted here to accept a mean and standart deviation,
 * which by default are 0 and 1.
 *
 * @param   mean        Expectation value of the distribution [default: 0.0]
 * @param   std_dev     Standart deviation of the distribution [default: 1.0]
 */
{
    static bool available = false;
    static double gset;
    double fac, r2, v1, v2;

    if (!available)
    {
        // If we don’t have an extra deviate handy
        // and while v1 and v2 are not in the unit circle.
        do
        {
            // Pick two uniform numbers in the square extending
            // from -1 to +1 in each direction,
            v1 = Get_Random_CloseN1_Close1();
            v2 = Get_Random_CloseN1_Close1();
            r2 = v1*v1 + v2*v2;
        } while (r2 >= 1.0 || r2 <= DBL_EPSILON);

        fac = sqrt(-2.0 * log(r2) / r2);
        // Now make the Box-Muller transformation to get two normal
        // deviates. Return one and save the other for next time.
        gset = mean + v1*fac * std_dev;

        // Set flag.
        available = true;

        return (mean + v2*fac * std_dev);
    }
    else
    {
        // We have an extra deviate handy, so unset the ﬂag and return it.
        available = false;
        return gset;
    }
}

// **************************************************************
double PRNG::Call_N_Time_Get_Random(const int n)
{
    std_cout << "PRNG Calling N=" << n << " times Get_Random()...\n" << std::flush;
    volatile double last_rand;
    for (int i = 0 ; i < n ; i++)
    {
        last_rand = Get_Random();
    }
    return last_rand;
}

// ********** End of file ***************************************
