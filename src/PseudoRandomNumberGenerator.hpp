#ifndef INC_PRNG_hpp
#define INC_PRNG_hpp

#include <stdint.h> // (u)int64_t
#include <cstdlib> // free()
#include <vector>

namespace prng
{
    // See Git_Info.cpp (generated dynamically from Git_Info.cpp_template & Makefile.rules)
    void Log_Git_Info(std::string basename = "");
}

// **************************************************************
// Pseudo-Random Number Generator's (PRNG)
class PRNG
{
    int is_initialized;
    int nb_calls;

    void *dsfmt_data;
    uint32_t seed;

    public:
                     PRNG();
                    ~PRNG();
        void        Initialize_Taking_Time_As_Seed(const bool quiet = false);
        void        Initialize(const uint32_t seed, const bool quiet = false);
        double      Get_Random();                   // Returns ]0,1]
        double      Get_Random_Close1_Open2();      // [1,2[ Most fundamental, others are based on it
        double      Get_Random_Close0_Open1();      // [0,1[
        double      Get_Random_Open0_Close1();      // ]0,1]
        double      Get_Random_CloseN1_Open1();     // [-1,1[
        double      Get_Random_Close0_Close1();     // [0,1]
        double      Get_Random_CloseN1_Close1();    // [-1,1]
        void        Get_Random_Direction(double xyz[3]);
        std::vector<double> Get_Random_Direction();
        double      gasdev();
        double      Get_Random_Box_Muller_Polar(const double mean = 0.0, const double std_dev = 1.0);
        double      Call_N_Time_Get_Random(const int n);
        uint32_t    Get_Seed()      { return seed;     }
        int         Get_Nb_Calls()  { return nb_calls; }
};

#endif // INC_PRNG_hpp

// ********** End of file ***************************************
