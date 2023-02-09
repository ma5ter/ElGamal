#include "elgamal.h"
#include "prime.h"

void elgamal_keygen(elgamal_keys_t *keys, int higher, uint64_t (*random64)(void)) {
	if (higher) {
		// generate new prime number & primitive root
		do {
			// large prime number p
			keys->p = random_prime_u64(higher, random64);

			// primitive root g modulo p
			keys->g = prime_find_primitive_root_u64(keys->p);
		} while (!keys->g);
	}

	// random private key x, 1 < x < p-1
	keys->x = random64() % (keys->p - 1) + 1;

	// calculate the public key y = g^x mod p
	keys->y = power_mod_u64(keys->g, keys->x, keys->p);
}

void elgamal_encrypt(uint64_t p, uint32_t g, uint64_t y, uint64_t message, uint64_t *out, uint64_t (*random64)(void)) {
	// choose a random number k, 1 < k < p-1
	uint64_t k = random64() % (p - 1) + 1;

	// compute a = g^k mod p
	out[0] = power_mod_u64(g, k, p);

	// compute b = (message * y^k) mod p
	out[1] = mul_mod_u64(message, power_mod_u64(y, k, p), p);
}

uint64_t elgamal_decrypt(uint64_t p, uint64_t x, uint64_t *in) {
	// compute m = b * (a^(p-1-x)) mod p
	uint64_t t = power_mod_u64(in[0], p - 1 - x, p);
	return mul_mod_u64(in[1],t, p);
}
