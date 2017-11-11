#include <string.h>
#include "utils.h"
#include "md5.h"

void MD5_Init(MD5_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
}

void MD5_transform(MD5_CTX *ctx)
{
    uint32_t s[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

	  uint32_t* M = (uint32_t*)(ctx->data);

	  uint32_t a = ctx->state[0];
	  uint32_t b = ctx->state[1];
	  uint32_t c = ctx->state[2];
	  uint32_t d = ctx->state[3];

	  for (uint32_t i = 0; i < 64; i++) {
			uint32_t f, g;

			if (i < 16) {
				 f = (b & c) | ((~b) & d);
				 g = i;
			} else if (i < 32) {
				 f = (d & b) | ((~d) & c);
				 g = (5 * i + 1) % 16;
			} else if (i < 48) {
				 f = b ^ c ^ d;
				 g = (3 * i + 5) % 16;
			} else {
				 f = c ^ (b | (~d));
				 g = (7 * i) % 16;
			}

			f = f + a + K[i] + M[g];
			a = d;
			d = c;
			c = b;
			b = b + rotl32(f, s[i]);
	  }

	  ctx->state[0] += a;
	  ctx->state[1] += b;
	  ctx->state[2] += c;
	  ctx->state[3] += d;
}

void MD5_Update(MD5_CTX *ctx, const uint8_t *data, size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			MD5_transform(ctx);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void MD5_Final(MD5_CTX *ctx, uint8_t *md)
{
	size_t i;

	i = ctx->datalen;

	ctx->data[i++] = 0x80;
	if (ctx->datalen < 56) {
		memset(ctx->data + i, 0, 56 - i);
	} else if (ctx->datalen >= 56) {
		ctx->data[i++] = 0x80;
		memset(ctx->data + i, 0, 64 - i);
		MD5_transform(ctx);
		memset(ctx->data, 0, 56);
	}

	ctx->bitlen += ctx->datalen;
	memcpy(ctx->data + 56, &ctx->bitlen, 8); // in bits at the end of the buffer
	MD5_transform(ctx);

	for (i = 0; i < 4; ++i) {
		md[i]      = (ctx->state[0] >> (i * 8)) & 0x000000ff;
		md[i + 4]  = (ctx->state[1] >> (i * 8)) & 0x000000ff;
		md[i + 8]  = (ctx->state[2] >> (i * 8)) & 0x000000ff;
		md[i + 12] = (ctx->state[3] >> (i * 8)) & 0x000000ff;
	}
}

uint8_t* MD5(const uint8_t* buf, size_t n, uint8_t* md)
{
	 static uint8_t tmp_md[MD5_DIGEST_LENGTH];

	 MD5_CTX ctx;
	 MD5_Init(&ctx);
	 MD5_Update(&ctx, buf, n);
	 if (md) {
		 MD5_Final(&ctx, md);
		 return md;
	 } else {
		 MD5_Final(&ctx, tmp_md);
		 return tmp_md;
	 }
}
