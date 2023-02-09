#include "prime.h"

// #define DEBUG_FACTORS
#ifdef DEBUG_FACTORS
#include <stdio.h>
#endif

uint64_t sqrt_u64(const uint64_t x) {
	return (int64_t)sqrt((double)x);
}

uint64_t mul_mod_u64(uint64_t a, uint64_t b, const uint64_t mod) {
	uint64_t result = 0;

	if (b < a) {
		const uint64_t t = a;
		a = b;
		b = t;
	}

	while (a) {
		if (a & 1) {
			uint64_t sum = result + b;
			// check for overflow
			if (sum < result || sum < b) {
				uint64_t hi = (0 - mod) % mod;
				uint64_t lo = sum % mod;
				sum = lo + hi;
			}
			result = sum % mod;
		}
		a >>= 1;
		// check for overflow
		uint64_t c = (b << 1) % mod;
		if (b & 0x8000000000000000ull) {
			uint64_t hi = (0 - mod) % mod;
			c += hi;
			c %= mod;
		}
		b = c;
	}

	return result;
}

uint64_t power_mod_u64(uint64_t base, uint64_t exp, const uint64_t mod) {
	uint64_t result = 1ull;

	while (exp) {
		if (exp & 1) {
			result = mul_mod_u64(result, base, mod);
		}
		base = mul_mod_u64(base, base, mod);
		exp >>= 1;
	}

	return result;
}

uint32_t prime_primitive_root(uint64_t mod) {
	// the only even prime has root of 1
	if (2 == mod) return 1;
	// if mod is prime then Euler's totient function s = p - 1
	const uint64_t phi = mod - 1;
	uint32_t limit = sqrt_u64(phi);
	uint64_t n = phi;
	int prime_factors_count = 0;
	#define prime_factors_max 10
	uint32_t prime_factors[prime_factors_max];

	for (uint32_t i = 2; i && i <= limit; ++i) {
		if (0 == n % i) {
			if (prime_factors_count >= prime_factors_max) return 0;
			prime_factors[prime_factors_count++] = i;
			#ifdef DEBUG_FACTORS
			printf("factor %u is %u\n", prime_factors_count, i);
			#endif
			do n /= i; while (0 == n % i);
		}
	}

	#ifdef DEBUG_FACTORS
	if (n > 1) printf("factor %u is %llu\n", prime_factors_count + 1, n);
	#endif

	// limit 32-bitters only
	limit = mod > UINT32_MAX ? UINT32_MAX : (uint32_t)mod;
	for (uint32_t g = 2; g <= limit; ++g) {
		// 4 is not a root for any prime
		if (4 == g) continue;
		if (n > 1 && power_mod_u64(g, phi / n, mod) == 1) continue;
		for (int i = 0; i < prime_factors_count; ++i) {
			if (power_mod_u64(g, phi / prime_factors[i], mod) == 1) goto do_continue;
		}
		return g;
		do_continue:;
	}

	return 0;
}

int is_prime_u64(const uint64_t x) {
	// trivial cases
	if (2 == x) return 1;
	if (0 == (x & 1) || 1 == x) return 0;

	const uint64_t limit = sqrt_u64(x);
	for (uint64_t i = 2; i <= limit; i++) {
		if (x % i == 0) {
			return 0;
		}
	}

	return 1;
}

// helper to the Miller-Rabin prime test
// that computes n−1 = 2^s * d with d odd by factoring powers of 2 from n−1
static int miller_rabin(const uint64_t n, const uint64_t a, const uint64_t d, int rounds) {
	const uint64_t y = n - 1;
	uint64_t x = power_mod_u64(a, d, n);

	if (x == 1 || x == y) {
		return 1;
	}

	while (rounds--) {
		x = mul_mod_u64(x, x, n);
		if (x == y) {
			return 1;
		}
	}

	return 0;
}

int is_probable_prime_u64(const uint64_t x) {
	// trivial cases
	if (2 == x) return 1;
	if (0 == (x & 1) || 1 == x) return 0;

	uint64_t d = x - 1;
	int r = 0;
	while (0 == (d & 1)) {
		r++;
		d >>= 1;
	}

	// deterministic variant
	// if n < 341,550,071,728,321, it is enough to test a = 2, 3, 5, 7, 11, 13, and 17.
	if (x >= 3474749660383ull) {
		return miller_rabin(x, 2, d, r) && miller_rabin(x, 3, d, r) \
			&& miller_rabin(x, 5, d, r) && miller_rabin(x, 7, d, r) \
			&& miller_rabin(x, 11, d, r) &&	miller_rabin(x, 13, d, r) \
			&& miller_rabin(x, 17, d, r);
	}
	// if n < 3,474,749,660,383, it is enough to test a = 2, 3, 5, 7, 11, and 13;
	if (x >= 2152302898747ull) {
		return miller_rabin(x, 2, d, r) && miller_rabin(x, 3, d, r) \
			&& miller_rabin(x, 5, d, r) && miller_rabin(x, 7, d, r) \
			&& miller_rabin(x, 11, d, r) && miller_rabin(x, 13, d, r);
	}
	// if n < 2,152,302,898,747, it is enough to test a = 2, 3, 5, 7, and 11;
	if (x >= 1122004669633ull) {
		return miller_rabin(x, 2, d, r) && miller_rabin(x, 3, d, r) \
			&& miller_rabin(x, 5, d, r) && miller_rabin(x, 7, d, r) \
			&& miller_rabin(x, 11, d, r);
	}
	// if n < 1,122,004,669,633, it is enough to test a = 2, 13, 23, and 1662803;
	if (x >= 4759123141ull) {
		return miller_rabin(x, 2, d, r) && miller_rabin(x, 13, d, r) \
		&& miller_rabin(x, 23, d, r) && miller_rabin(x, 1662803, d, r);
	}
	// if n < 4,759,123,141, it is enough to test a = 2, 7, and 61;
	if (x >= 9080191ull) {
		return miller_rabin(x, 2, d, r) && miller_rabin(x, 7, d, r) \
		&& miller_rabin(x, 61, d, r);
	}
	// if n < 9,080,191, it is enough to test a = 31 and 73;
	if (x >= 1373653ull) {
		return miller_rabin(x, 31, d, r) && miller_rabin(x, 73, d, r);
	}
	// if n < 1,373,653, it is enough to test a = 2 and 3;
	return miller_rabin(x, 2, d, r) && miller_rabin(x, 3, d, r);
}

uint64_t random_prime_u64(int higher, uint64_t (*random64)(void)) {
	if (higher > 56) higher = 56;

	uint64_t mask = (((1ull << higher) - 1) << (64 - higher)) | 1;
	uint64_t result = random64() | mask;

	// find next prime number
	while (!is_probable_prime_u64(result)) {
		result++;
		// make odd, deals with overflow also
		result |= mask;
	}

	return result;
}