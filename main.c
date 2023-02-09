#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prime.h"
#include "elgamal.h"

/// using weak random for tests
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-msc51-cpp"

// weak random
uint64_t random64(void) {
	#if RAND_MAX < 0xFFFF
	return (((uint64_t)rand() ^ ((uint64_t)rand() << 3)) << 48)
		| (((uint64_t)rand() ^ ((uint64_t)rand() << 3)) << 32)
		| (((uint64_t)rand() ^ ((uint64_t)rand() << 3)) << 16)
		| ((uint64_t)rand() ^ ((uint64_t)rand()<< 3));
	#elif RAND_MAX < 0xFFFFFFFF
	return (((uint64_t)rand() ^ ((uint64_t)rand() << 3)) << 32)
		| ((uint64_t)rand() ^ ((uint64_t)rand() << 3));
	#else
	return (uint64_t)rand() ^ ((uint64_t)rand() << 3);
	#endif
}

int main() {
	srand(time(NULL));

	// printf("prime = %d\n", is_prime_u64(18446744073709551557ull));
	// printf("%llu", mul_mod_u64(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull));
	// printf("prime = %d\n", is_probable_prime_u64(18446744073709551557ull));

	// uint64_t x = random_prime_u64(32, random64);
	// printf("x = %llu (0x%016llX)\n", x, x);
	// printf("prime = %d\n", is_prime_u64(x));

	// printf("%u\n", prime_primitive_root(157));
	// printf("%u\n", prime_primitive_root(x));

	elgamal_keys_t keys;
	elgamal_keygen(&keys, 32, random64);

	uint64_t m = 12345678901234567890ull;
	uint64_t buf[2];
	elgamal_encrypt(keys.p, keys.g, keys.y, m, buf, random64);
	m = elgamal_decrypt(keys.p, keys.x, buf);

	printf("m = %llu", m);

	return 0;
}
