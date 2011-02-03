/***************************************************************
 *
 *
 ***************************************************************/

#include <cstdlib>
#include <iostream>
#include <stdint.h> // (u)int64_t
// #include <fstream>

#include <PseudoRandomNumberGenerator.hpp>
#include <Memory.hpp>

const unsigned int N = 10;
const uint32_t seed  = 0;

void generate_input_file();

// **************************************************************
int main(int argc, char *argv[])
{

    generate_input_file();

    return EXIT_SUCCESS;
}


// **************************************************************
void generate_input_file()
/**
 * Generate an array to save to a file for later use
 */
{
    std::cout << "**************************************************************\n";
    std::cout << "Generating an array of " << N << " doubles to save to a file for later use...\n";
    PRNG prng;
    prng.Initialize(seed, false); // quiet = false

//     prng.Call_N_Time_Get_Random(10000);

    char output_filename[1000];
    sprintf(output_filename, "output/N%u.txt", N);

    std::ofstream file;
    file.open(output_filename);

    std::cout << "\n" << std::flush;
    for (unsigned int i = 0 ; i < N ; i++)
    {
//         //std::cout << "\r        \r" <<
        std::cout << "i = " << i << "\n" << std::flush;
        //file << prng.Get_Random() << "\n" << std::flush;
        std::cout << "  " << prng.Get_Random() << "\n" << std::flush;
    }

    file.close();

    std::cout << N << " random doubles have been saved to \"" << output_filename  << "\"\n";
    std::cout << "Please copy this file into the input/ folder.\n";


    std::cout << "prng.Get_Random() called " << prng.Get_Nb_Calls() << "\n" << std::flush;
}


