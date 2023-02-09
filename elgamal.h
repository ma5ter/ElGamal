#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <stdint.h>

typedef struct elgamal_keys {
	// p: prime number
	uint64_t p;
	// g: primitive root modulo p
	uint32_t g;
	// x: private key
	uint64_t x;
	// y: public key
	uint64_t y;
} elgamal_keys_t;

/// Generates private and public keys
/// and if needed prime number & primitive root for ElGamal scheme
/// \param keys - out - generated keys
/// \param higher - number of higher bits of prime key part to be set to 1,
/// when 'higher' is zero generation of the prime number & primitive root is skipped
/// \param random64 - random generator function pointer which returns 64-bit randoms
void elgamal_keygen(elgamal_keys_t *keys, int higher, uint64_t (*random64)(void));

/// ElGamal encryption routine
/// \param p - prime number
/// \param g - primitive root
/// \param y - public key
/// \param message - message to encrypt
/// \param out - encrypted output
/// \param random64 - random generator function pointer which returns 64-bit randoms
void elgamal_encrypt(uint64_t p, uint32_t g, uint64_t y, uint64_t message, uint64_t *out, uint64_t (*random64)(void));

/// ElGamal decryption routine
/// \param p - prime number
/// \param x - private key
/// \param in - encrypted input
/// \return decrypted message
uint64_t elgamal_decrypt(uint64_t p, uint64_t x, uint64_t *in);

#endif
