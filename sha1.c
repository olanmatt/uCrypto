#include <string.h>
#include "utils.h"
#include "sha1.h"

void SHA1_Init(SHA1_CTX *ctx)
{
   ctx->datalen = 0;
   ctx->bitlen = 0;
   ctx->state[0] = 0x67452301;
   ctx->state[1] = 0xEFCDAB89;
   ctx->state[2] = 0x98BADCFE;
   ctx->state[3] = 0x10325476;
   ctx->state[4] = 0xC3D2E1F0;
}

void SHA1_transform(SHA1_CTX *ctx)
{
   uint32_t w[80];
   for (int i=0, j=0; i < 16; ++i, j += 4)
      w[i] = (ctx->data[j] << 24) + (ctx->data[j+1] << 16) + (ctx->data[j+2] << 8) + (ctx->data[j+3]);

   for (int i = 16; i < 80; i++) {
      w[i] = rotl32(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
   }

   uint32_t a = ctx->state[0];
   uint32_t b = ctx->state[1];
   uint32_t c = ctx->state[2];
   uint32_t d = ctx->state[3];
   uint32_t e = ctx->state[4];

   for (uint32_t i = 0; i < 80; i++) {
      uint32_t f, k;
      if (i < 20) {
         f = (b & c) | ((~b) & d);
         k = 0x5A827999;
      } else if (i < 40) {
         f = b ^ c ^ d;
         k = 0x6ED9EBA1;
      } else if (i < 60) {
         f = (b & c) | (b & d) | (c & d);
         k = 0x8F1BBCDC;
      } else {
         f = b ^ c ^ d;
         k = 0xCA62C1D6;
      }

      uint32_t temp = rotl32(a, 5) + f + e + k + w[i];
      e = d;
      d = c;
      c = rotl32(b, 30);
      b = a;
      a = temp;
   }

   ctx->state[0] += a;
   ctx->state[1] += b;
   ctx->state[2] += c;
   ctx->state[3] += d;
   ctx->state[4] += e;
}

void SHA1_Update(SHA1_CTX *ctx, const uint8_t *data, size_t len)
{
   for (size_t i = 0; i < len; ++i) {
      ctx->data[ctx->datalen] = data[i];
      ctx->datalen++;
      if (ctx->datalen == 64) {
         SHA1_transform(ctx);
         ctx->bitlen += 512;
         ctx->datalen = 0;
      }
   }
}

void SHA1_Final(SHA1_CTX *ctx, uint8_t *md)
{
   size_t i;

   i = ctx->datalen;

   ctx->data[i++] = 0x80;
   if (ctx->datalen < 56) {
      memset(ctx->data + i, 0, 56 - i);
   } else if (ctx->datalen >= 56) {
      memset(ctx->data + i, 0, 64 - i);
      SHA1_transform(ctx);
      memset(ctx->data, 0, 56);
   }

	ctx->bitlen += ctx->datalen * 8;
	uint64_t bitlen = byteswap64(ctx->bitlen);
	
   memcpy(ctx->data + 56, &bitlen, 8);
   SHA1_transform(ctx);

   for (int i=0; i < 4; ++i) {
      md[i]    = (ctx->state[0] >> (24-i*8)) & 0x000000ff;
      md[i+4]  = (ctx->state[1] >> (24-i*8)) & 0x000000ff;
      md[i+8]  = (ctx->state[2] >> (24-i*8)) & 0x000000ff;
      md[i+12] = (ctx->state[3] >> (24-i*8)) & 0x000000ff;
      md[i+16] = (ctx->state[4] >> (24-i*8)) & 0x000000ff;
   }
}

uint8_t* SHA1(const uint8_t* buf, size_t n, uint8_t* md)
{
	static uint8_t tmp_md[SHA1_DIGEST_LENGTH];

   SHA1_CTX ctx;
   SHA1_Init(&ctx);
	SHA1_Update(&ctx, buf, n);
	if (md) {
		SHA1_Final(&ctx, md);
		return md;
	} else {
		SHA1_Final(&ctx, tmp_md);
		return tmp_md;
	}
}
