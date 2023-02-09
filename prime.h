#ifndef PRIME_H
#define PRIME_H

#include <stdint.h>
#include <math.h>

/// An integer helper to the square root that deals higher bit output relaxed
/// Without implicit conversion into int64_t the assembly code is much complex
/// as Intel for most platforms only provides 64-bit signed integer to double
/// precision conversion (and the opposite), but not for the unsigned.
/// They added the unsigned conversion instruction only in AVX-512.
/// \param x - unsigned 64-bit number
/// \return integer square root
uint64_t sqrt_u64(uint64_t x);

/// Computes (a * b) % n for 64-bit unsigned integers
/// \param a - first multiplicand unsigned 64-bit number
/// \param b - second multiplicand unsigned 64-bit number
/// \param mod modulus unsigned 64-bit number
/// \return (a * b) % n
uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod);

/// Computes (base^exp) % mod
/// \param base - base unsigned 64-bit number
/// \param exp - exponent unsigned 64-bit number
/// \param mod - modulus unsigned 64-bit number
/// \return (base^exp) % mod
uint64_t power_mod_u64(uint64_t base, uint64_t exp, uint64_t mod);

/// Splits 64-bit unsigned number into prime factors
/// \attention All the factors are returned as 32-bit unsigned integers except one
/// which is returned in the 'last_factor' out parameter and set either to 1 or the
/// last factor value. Note that the last factor is not counted with the return
/// value of the function and should be determined by the value of the 'last_factor'
/// \param x - 64 bit unsigned number to be split
/// \param last_factor - out - the only 64-bit factor if exist or 1 if no
/// \param factors - out - array of 32-bit factors
/// \param factors_max - length of the array
/// \return number of 32-bit factors, if length of the array is less than the
/// number of 32-bit factors count then 0 returned instead
size_t find_prime_factors_u64(uint64_t x, uint64_t *last_factor, uint32_t *factors, size_t factors_max);

/// Returns minimal primitive root modulo for a PRIME(!) mod
/// \param mod - prime unsigned 64-bit number
/// \param min - first root to start search from
/// \param max - last root to stop search
/// \return primitive root
uint32_t prime_find_primitive_root_u64(uint64_t mod, uint32_t min, uint32_t max);

/// A straightforward algorithm to check if a 64-bit unsigned integer is a prime
/// number is the trial division method, where you divide the number by all ints
/// greater than 1 and less than or equal to the square root of the number.
/// \param x - unsigned 64-bit value
/// \return '0' if x is not a prime number
int is_prime_u64(uint64_t x);

/// Implementation of the Miller-Rabin primality test for large 64-bit primes.
/// Function checks if a number is prime by performing the Miller-Rabin test
/// 'tests' times and returning 0 if the number fails any test.
/// \param x - unsigned 64-bit number
/// \return '0' if x is not a prime number
int is_probable_prime_u64(uint64_t x);

/// Generates a large 64-bit prime number
/// \param higher number of higher bits of result to be set to 1
/// \param random64 random generator function pointer which returns 64-bit randoms
/// \return large 64-bit prime number with specified number of higher bits set
uint64_t random_prime_u64(int higher, uint64_t (*random64)(void));

#endif
