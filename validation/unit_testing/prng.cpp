#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <cmath>

#include <PseudoRandomNumberGenerator.hpp>
#include <Memory.hpp>

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random();
        BOOST_CHECK_MESSAGE(r   <= 1.0, "prng.Get_Random() must be less or equal than 1");
        BOOST_CHECK_MESSAGE(0.0 <  r,   "prng.Get_Random() must be greater than 0");
    }
}

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random_Close1_Open2)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random_Close1_Open2();
        BOOST_CHECK_MESSAGE(1.0 <= r,   "prng.Get_Random_Close1_Open2() must be greater or equal to 1");
        BOOST_CHECK_MESSAGE(r   <  2.0, "prng.Get_Random_Close1_Open2() must be less than 2");
    }
}

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random_Close0_Open1)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random_Close0_Open1();
        BOOST_CHECK_MESSAGE(0.0 <= r,   "prng.Get_Random_Close0_Open1() must be greater or equal to 0");
        BOOST_CHECK_MESSAGE(r   <  1.0, "prng.Get_Random_Close0_Open1() must be less than 1");
    }
}

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random_Open0_Close1)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random_Open0_Close1();
        BOOST_CHECK_MESSAGE(0.0 <  r,   "prng.Get_Random_Open0_Close1() must be greater to 0");
        BOOST_CHECK_MESSAGE(r   <= 1.0, "prng.Get_Random_Open0_Close1() must be less or equal than 1");
    }
}

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random_CloseN1_Open1)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random_CloseN1_Open1();
        BOOST_CHECK_MESSAGE(-1.0 <= r,   "prng.Get_Random_CloseN1_Open1() must be greater or equal to -1");
        BOOST_CHECK_MESSAGE(r    <  1.0, "prng.Get_Random_CloseN1_Open1() must be less than 1");
    }
}

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random_Close0_Close1)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random_Close0_Close1();
        BOOST_CHECK_MESSAGE(0.0 <= r,   "prng.Get_Random_Close0_Close1() must be greater or equal to 0");
        BOOST_CHECK_MESSAGE(r   <= 1.0, "prng.Get_Random_Close0_Close1() must be less or equal than 1");
    }
}

BOOST_AUTO_TEST_CASE(PRNG_Seed_Time_Get_Random_CloseN1_Close1)
{
    // Initialize object
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed(true); // quiet == true

    for (int i = 0 ; i < 10000 ; i++)
    {
        const double r = prng.Get_Random_CloseN1_Close1();
        BOOST_CHECK_MESSAGE(-1.0 <= r,   "prng.Get_Random_CloseN1_Close1() must be greater or equal to -1");
        BOOST_CHECK_MESSAGE(r    <= 1.0, "prng.Get_Random_CloseN1_Close1() must be less or equal than 1");
    }
}


BOOST_AUTO_TEST_CASE(PRNG_Reloading)
{
    const int N = 1000000;
    uint32_t seed;

    // Populate a large array
    double *array_original = (double *) calloc_and_check(N, sizeof(double));
    {
        // "prng" is local to this block
        PRNG prng;
        prng.Initialize_Taking_Time_As_Seed(true); // quiet == true
        seed = prng.Get_Seed();

        for (int i = 0 ; i < N ; i++)
            array_original[i] = prng.Get_Random(); // ]0,1]
    }

    // Populate a second array with the same seed as the original one
    double *array_second = (double *) calloc_and_check(N, sizeof(double));
    {
        // "prng" is local to this block
        PRNG prng;
        prng.Initialize(seed, true); // quiet == true

        for (int i = 0 ; i < N ; i++)
            array_second[i] = prng.Get_Random(); // ]0,1]
    }

    // Make sure all elements are the same
    for (int i = 0 ; i < N ; i++)
    {
        BOOST_CHECK_MESSAGE(std::abs(array_second[i] - array_original[i]) < 1.0e-14, "Two PRNG arrays using same seed are not the same!");
    }
}

