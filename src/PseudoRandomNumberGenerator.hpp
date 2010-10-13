#ifndef INC_PRNG_hpp
#define INC_PRNG_hpp

#include <stdint.h> // (u)int64_t
#include <cstdlib> // free()

// **************************************************************
// Pseudo-Random Number Generator's (PRNG)
const int Lib_PRNG_TYPE_CPP   = 0;
const int Lib_PRNG_TYPE_DSFMT = 1;
class Lib_PRNG
{
    int type;
    int is_initialized;
    int nb_calls;

    // Use void pointers to prevent poluting main code with dSFMT
    void *dsfmt_data;

    uint32_t seed;

    public:
        void        Initialize(const uint32_t seed);
        double      Get_Random();                   // Returns ]0,1]
        double      Get_Random_Close1_Open2();      // [1,2[ Most fundamental, others are based on it
        double      Get_Random_Close0_Open1();      // [0,1[
        double      Get_Random_Open0_Close1();      // ]0,1]
        double      Get_Random_CloseN1_Open1();     // [-1,1[
        double      Get_Random_Close0_Close1();     // [0,1]
        double      Get_Random_CloseN1_Close1();    // [-1,1]
        void        Get_Random_Direction(double xyz[3]);
        double      gasdev();
        double      Get_Random_Box_Muller_Polar(const double mean = 0.0, const double std_dev = 1.0);
        double      Call_N_Time_Get_Random(const int n);
        uint32_t    Get_Seed()      { return seed;     }
        int         Get_Nb_Calls()  { return nb_calls; }
                    ~Lib_PRNG()     { free(dsfmt_data); }
};

#endif // INC_PRNG_hpp

// ********** End of file ***************************************
