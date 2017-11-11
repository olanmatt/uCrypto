#include <string.h>
#include "utils.h"
#include "sha1.h"

#include <stdio.h>

void sha256(const uint8_t* buf, size_t n, uint8_t* res)
{
	uint32_t h0 = 0x6a09e667;
	uint32_t h1 = 0xbb67ae85;
	uint32_t h2 = 0x3c6ef372;
	uint32_t h3 = 0xa54ff53a;
	uint32_t h4 = 0x510e527f;
	uint32_t h5 = 0x9b05688c;
	uint32_t h6 = 0x1f83d9ab;
	uint32_t h7 = 0x5be0cd19;

	uint32_t k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	int pad_n = (n * 8) + 1;

	if (pad_n % 512 > 448)
		pad_n += 512 - (pad_n % 512);

	if (pad_n % 512 < 448)
		pad_n += 448 - (pad_n % 512);

	pad_n /= 8;

	uint8_t* pad_buf = (uint8_t*)calloc(pad_n + 64, 1);

	memcpy(pad_buf, buf, n);

	pad_buf[n] = 0x80;

	uint64_t bits_len = byteswap64(8 * n); // note, we append the len
	memcpy(pad_buf + pad_n, &bits_len, 8); // in bits at the end of the buffer

	for (int offset = 0; offset < pad_n; offset += (512 / 8)) {
		uint32_t w[64];

		uint8_t* data = &pad_buf[offset];
		for (int i=0, j=0; i < 16; ++i, j += 4)
			w[i] = (data[j] << 24) + (data[j+1] << 16) + (data[j+2] << 8) + (data[j+3]);

		for (int i = 16; i < 64; i++) {
			uint32_t s0 = rotr32(w[i-15], 7) ^ rotr32(w[i-15], 18) ^ (w[i-15] >> 3);
			uint32_t s1 = rotr32(w[i-2], 17) ^ rotr32(w[i-2], 19) ^ (w[i-2] >> 10);
			w[i] = w[i-16] + s0 + w[i-7] + s1;
		}

		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;
		uint32_t f = h5;
		uint32_t g = h6;
		uint32_t h = h7;

		for (int i = 0; i < 64; i++) {
			uint32_t S1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
			uint32_t ch = (e & f) ^ ((~e) & g);
			uint32_t temp1 = h + S1 + ch + k[i] + w[i];
			uint32_t S0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t temp2 = S0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
		h5 += f;
		h6 += g;
		h7 += h;
	}

	for (int i=0; i < 4; ++i) {
		res[i]    = (h0 >> (24-i*8)) & 0x000000ff;
		res[i+4]  = (h1 >> (24-i*8)) & 0x000000ff;
		res[i+8]  = (h2 >> (24-i*8)) & 0x000000ff;
		res[i+12] = (h3 >> (24-i*8)) & 0x000000ff;
		res[i+16] = (h4 >> (24-i*8)) & 0x000000ff;
		res[i+20] = (h5 >> (24-i*8)) & 0x000000ff;
		res[i+24] = (h6 >> (24-i*8)) & 0x000000ff;
		res[i+28] = (h7 >> (24-i*8)) & 0x000000ff;
	}
}
