#ifndef INC_MEMORY_hpp
#define INC_MEMORY_hpp

#include <climits> // CHAR_BIT
#include <stdint.h> // (u)int64_t

#include <StdCout.hpp>

// **************************************************************
template <class Integer>
static inline void * calloc_and_check(Integer nb, size_t s, const std::string msg = "")
{
    void *p = NULL;
    p = calloc(nb, s);
    if (p == NULL)
    {
        double nb_s = double(nb) * double(s);
        std_cout << "Allocation of " << nb << " x " << s << " bytes = " << nb_s << " bytes failed" << std::endl;
        std_cout << "(" << double(nb_s) / (1024.0) << " KiB, "
                         << double(nb_s) / (1024.0*1024.0) << " MiB, "
                         << double(nb_s) / (1024.0*1024.0*1024.0) << " GiB)" << std::endl;
        std_cout << "p = " << p << "\n";
        if (msg != "")
        {
            std_cout << "Comment: " << msg << std::endl;
        }
        std_cout << "Aborting\n" << std::flush;
        abort();
    }
    return p;
}

// **************************************************************
static inline void * malloc_and_check(size_t nb_s, const std::string msg = "")
{
    void *p = NULL;
    p = malloc(nb_s);
    if (p == NULL)
    {
        std_cout << "Allocation of " << nb_s << " bytes failed" << std::endl;
        std_cout << "(" << double(nb_s) / (1024.0) << " KiB, "
                         << double(nb_s) / (1024.0*1024.0) << " MiB, "
                         << double(nb_s) / (1024.0*1024.0*1024.0) << " GiB)" << std::endl;
        std_cout << "p = " << p << "\n";
        if (msg != "")
        {
            std_cout << "Comment: " << msg << std::endl;
        }
        std_cout << "Aborting\n" << std::flush;
        abort();
    }
    return p;
}

// **************************************************************
template <class Integer>
static inline void * malloc_and_check(Integer nb, size_t s, const std::string msg = "")
{
    return malloc_and_check(nb * s, msg);
}

// **************************************************************
template <class Pointer>
void free_me(Pointer &p)
{
    if (p != NULL)
    {
        free(p);
    }
    p = NULL;
}

// **************************************************************
template <class Integer>
void Print_Integer_in_Binary(Integer n)
/**
 * Prints binary representation of an integer if any size.
 * Inspired by http://www.exploringbinary.com/displaying-the-raw-fields-of-a-floating-point-number/
 * WARNING: In C/C++, logical right shift of SIGNED integers is compiler dependant. GCC keeps the
 *          sign bit intact (instead of putting a 0).
 *          So ">>" is an arithmetic shift when the integer is signed. Unsigned are not
 *          affected (arithmetic and logical shifts are the same for unsigned integers).
 *          See http://en.wikipedia.org/wiki/Bitwise_operation#Arithmetic_shift
 */
{
                                        // Example 32 bits integers, converted from
                                        // http://www.binaryconvert.com/convert_unsigned_int.html
    const Integer zero  =  Integer(0);  // 00000000 00000000 00000000 00000000
    //const Integer ones  = ~zero;        // 11111111 11111111 11111111 11111111
    const Integer one   =  Integer(1);  // 00000000 00000000 00000000 00000001
    //const Integer two   =  Integer(2);  // 00000000 00000000 00000000 00000010
    //const Integer eigth =  Integer(8);  // 00000000 00000000 00000000 00001000
    const Integer nb_bits_per_byte    = CHAR_BIT; // Normaly, it is 8, but could be different.
    const Integer nb_bits_per_Integer = sizeof(n)*nb_bits_per_byte;

    // Starting from the LSB being index "0", the MSB is at index "msb_position"
    const Integer msb_position  = nb_bits_per_Integer - one;
    const Integer msb           = one << msb_position;
    const Integer or_msb        = ~msb;

    // Note that right shifting a signed integer migth keep the sign bit intact
    // (instead of setting it to 0) because C/C++ is implementation dependant
    // regarding right shift applied to negative signed integers. GCC will do
    // an "arithmetic right shift", meaning dividing the integer by 2. This will
    // keep the number negative (if it was). Because of this, the mask can get
    // screwed. If the Integer type is signed, first right shifting of the
    // mask of one (having an initial value of "msb" == 10000... and thus a
    // negative value) will keep the sign bit (leading to mask == 11000...) but
    // what we want is just to move the mask's bit, not keep the integer
    // reprentation "valid" (we want mask == 01000...). To fix that, after
    // right shifting the mask by one, we "AND" it (using "&") with "or_msb"
    // (or_msb == 01111...) to make sure we forget the sign bit.
    for (Integer mask = msb ; mask != zero ; mask = ((mask >> one) & or_msb ))
    {
        // If "n"'s bit at position of the mask is 0, print 0, else print 1.
        if ((mask & n) == zero) std_cout << "0";
        else                    std_cout << "1";
    }
}

// **************************************************************
static inline void Print_Double_in_Binary(double d)
/**
 * Prints binary representation of a double
 * http://www.exploringbinary.com/displaying-the-raw-fields-of-a-floating-point-number/
 */
{
    uint64_t *double_as_int = (uint64_t *) &d;
    const int bit_size = CHAR_BIT*sizeof(d);

    // Print bits by bits
    for (int b = 0 ; b <= bit_size-1 ; b++)
    {
        if (b == 1)
            std_cout << " ";    // Space after sign field
        if (b == 12)
            std_cout << " ";    // Space after exponent field

        // Get bit, but in reverse order. On Little Endian machines
        // (most of Intel and such), the byte with lower address
        // is the less significant. Since we want to print from
        // the most significant, we iterate from the end.
        if ((*double_as_int >> ((bit_size-1)-b)) & 1)
            std_cout << "1";
        else
            std_cout << "0";
    }
    //std_cout << "\n";
}

// **************************************************************
static inline void Print_Float_in_Binary(float d)
/**
 * Prints binary representation of a double
 * http://www.exploringbinary.com/displaying-the-raw-fields-of-a-floating-point-number/
 */
{
    uint32_t *float_as_int = (uint32_t *) &d;
    const int bit_size = CHAR_BIT*sizeof(d);

    // Print bits by bits
    for (int b = 0 ; b <= bit_size-1 ; b++)
    {
        if (b == 1)
            std_cout << " ";    // Space after sign field
        if (b == 9)
            std_cout << " ";    // Space after exponent field

        // Get bit, but in reverse order. On Little Endian machines
        // (most of Intel and such), the byte with lower address
        // is the less significant. Since we want to print from
        // the most significant, we iterate from the end.
        if ((*float_as_int >> ((bit_size-1)-b)) & 1)
            std_cout << "1";
        else
            std_cout << "0";
    }
    //std_cout << "\n";
}

// **************************************************************
static inline void Print_Double_in_Binary(float d)
{
    Print_Float_in_Binary(d);
}


#endif // INC_MEMORY_hpp

// ********** End of file ***************************************
