# PRNG
PRNG, a C++ library for pseudo-random number generator. It is mainly a wrapper
around either C/C++'s native **rand()** (which you should _NOT_ use) or dSFMT,
the double-precision SIMD-oriented Fast Mersenne Twister from
http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/ (version 2.9).

Since using the same seed twice will result in the exact same pseudo-random
number sequence, it is possible to use this library in simulations that need
reloading from snapshots. One just needs to save the seed and the number of times
a pseudo-random number was generated (using Get_Seed() and Get_Nb_Calls()),
re-initialize the library using Initialize(seed) and use Call_N_Time_Get_Random(n)
to get to the exact location in the pseudo-random numbers sequence.

The following functions are defined:

* void Initialize_Taking_Time_As_Seed(const bool quiet = false)
Will initialize using the time as the seed.

* void Initialize(const uint32_t seed, const bool quiet = false);
Initialize using a specific seed.

* double Get_Random()
Returns a random number in the range ]0,1]

* double Get_Random_Close1_Open2()
Returns a random number in the range [1,2[

* double Get_Random_Close0_Open1()
Returns a random number in the range [0,1[

* double Get_Random_Open0_Close1()
Returns a random number in the range ]0,1]

* double Get_Random_CloseN1_Open1()
Returns a random number in the range [-1,1[

* double Get_Random_Close0_Close1()
Returns a random number in the range [0,1]

* double Get_Random_CloseN1_Close1()
Returns a random number in the range [-1,1]

* void Get_Random_Direction(double xyz[3])
Sets the array "xyz" to point in a random direction on the unit sphere (uses Marsaglia
internally).

* double Get_Random_Box_Muller_Polar(const double mean = 0.0, const double std_dev = 1.0)
Box-Muller in pollar coordinates.

* uint32_t Get_Seed()
Return the seed used.

* int Get_Nb_Calls()
Return the number of times a pseudo-random number was generated.

* double Call_N_Time_Get_Random(const int n)
Generates **n** pseudo-random numbers, thus advancing into the sequence.


# Compilation
Requirements: [https://github.com/nbigaouette/stdcout](stdcout) and
[https://github.com/nbigaouette/memory](memory).

To build and install:

``` bash
$ make gcc shared static install
```

By default, it will be installed in ${HOME}/usr. You can install somewhere else
using DESTDIR:

``` bash
$ make gcc shared static install DESTIR=/usr
```

Library name will be "libprng".


# Example

``` C++
    PRNG prng;
    prng.Initialize_Taking_Time_As_Seed();
    std::cout << "pseudo-random number: " << prng.Get_Random() << "\n";
    double direction[3];
    prng.Get_Random_Direction(direction);
    std::cout << "random direction: (" << direction[0] << ", " << direction[1] << ", " << direction[2] << ")\n";
```


# License

This code is distributed under the terms of the [GNU General Public License v3 (GPLv3)](http://www.gnu.org/licenses/gpl.html) and is Copyright 2011 Nicolas Bigaouette.
dSFMT is Copyright (c) 2006,2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
University. It's license can be found at http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/LICENSE.txt

