#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stdlib.h>

#define MD5_DIGEST_LENGTH 16

typedef struct {
	uint8_t data[64];
   uint32_t datalen;
   uint64_t bitlen;
   uint32_t state[4];
} MD5_CTX;

void MD5_Init(MD5_CTX *ctx);
void MD5_Update(MD5_CTX *ctx, const uint8_t *data, size_t len);
void MD5_Final(MD5_CTX *ctx, uint8_t *md);
uint8_t* MD5(const uint8_t* buf, size_t n, uint8_t* md);

#endif // MD5_H
