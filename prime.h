#ifndef PRIME_H
#define PRIME_H

#include <stdint.h>
#include <math.h>

/// An integer helper to the square root that deals higher bit output relaxed
/// Without implicit conversion into int64_t the assembly code is much complex
/// as Intel for most platforms only provides 64-bit signed integer to double
/// precision conversion (and the opposite), but not for the unsigned.
/// They added the unsigned conversion instruction only in AVX-512.
/// \param x - unsigned 64-bit value
/// \return - integer square root
uint64_t sqrt_u64(uint64_t x);

/// Computes (a * b) % n for 64-bit unsigned integers
/// \param a
/// \param b
/// \param mod - modulus
/// \return (a * b) % n
uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod);

/// Computes (base^exp) % mod
/// \param base
/// \param exp
/// \param mod
/// \return - (base^exp) % mod
uint64_t power_mod_u64(uint64_t base, uint64_t exp, uint64_t mod);

/// Returns minimal primitive root modulo mod
/// \param mod
/// \return - primitive root
uint32_t prime_primitive_root(uint64_t mod);

/// A straightforward algorithm to check if a 64-bit unsigned integer is a prime
/// number is the trial division method, where you divide the number by all ints
/// greater than 1 and less than or equal to the square root of the number.
/// \param x - unsigned 64-bit value
/// \return - '0' if x is not a prime number
int is_prime_u64(uint64_t x);

/// Implementation of the Miller-Rabin primality test for large 64-bit primes.
/// Function checks if a number is prime by performing the Miller-Rabin test
/// 'tests' times and returning 0 if the number fails any test.
/// \param x - unsigned 64-bit value
/// \return - '0' if x is not a prime number
int is_probable_prime_u64(uint64_t x);

/// Generates a large 64-bit prime number
/// \param higher - number of higher bits of result to be set to 1
/// \param random64 - random generator function pointer which returns 64-bit randoms
/// \return - large 64-bit prime number with specified number of higher bits set
uint64_t random_prime_u64(int higher, uint64_t (*random64)(void));

#endif
