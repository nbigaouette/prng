/***************************************************************
 *
 *
 ***************************************************************/

#include <cstdlib>
#include <iostream>
#include <stdint.h> // (u)int64_t
#include <cmath>

#include <PseudoRandomNumberGenerator.hpp>
#include <Memory.hpp>

const unsigned int N = 100000;
const uint32_t seed  = 0;

void generate_input_file();
void compare_previously_generated_file();

// **************************************************************
int main(int argc, char *argv[])
{

    generate_input_file();
    compare_previously_generated_file();

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
    std::cout << "Seed used: " << prng.Get_Seed() << "\n" << std::flush;

    char output_filename[1000];
    sprintf(output_filename, "output/N%u.txt", N);

    std::ofstream file;
    file.open(output_filename);
    if (!file.is_open())
    {
        std::cout << "Couldn't open file " << output_filename << " for writting. Aborting.\n" << std::flush;
        abort();
    }

    std::cout << "Saving seed...\n" << std::flush;
    file << prng.Get_Seed() << "\n" << std::flush;

    file << std::setw(16);
    file << std::setprecision(14);

    for (unsigned int i = 0 ; i < N ; i++)
    {
        file << prng.Get_Random() << "\n" << std::flush;
    }

    file.close();

    std::cout << N << " random doubles have been saved to \"" << output_filename  << "\"\n";
    std::cout << "Please copy this file into the input/ folder.\n";
}

// **************************************************************
void compare_previously_generated_file()
{
    std::cout << "**************************************************************\n";
    std::cout << "Comparing previously generated array of " << N << " doubles to new one...\n";
    PRNG prng;

    char input_filename[1000];
    sprintf(input_filename, "input/N%u.txt", N);

    std::ifstream file;
    file.open(input_filename);
    if (!file.is_open())
    {
        std::cout << "Couldn't open file " << input_filename << " for reading. Aborting.\n" << std::flush;
        abort();
    }

    std::cout << "Reading seed...\n" << std::flush;
    uint32_t seed_read;
    file >> seed_read;
    prng.Initialize(seed_read, false); // quiet = false
    std::cout << "Seed used: " << prng.Get_Seed() << "\n" << std::flush;

    double rand_read;
    double rand_generated;
    double diff;
    const double tolerance = 1.0e-15;
    unsigned int nb_errors = 0;
    for (unsigned int i = 0 ; i < N ; i++)
    {
        file >> rand_read;
        rand_generated = prng.Get_Random();

        diff = rand_read - rand_generated;
        if (std::abs(diff) > 1.0e-14)
        {
            std::cout << "ERROR: Read value of " << rand_read << " to different from calculated value of " << rand_generated << ". Diff = " << diff << "\n" << std::flush;
            ++nb_errors;
        }
    }

    std::cout << "Comparison done. " << nb_errors << " error" << (nb_errors > 1 ? "s" : "") << "\n" << std::flush;

    file.close();
}

