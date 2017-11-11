#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stdlib.h>

#define SHA1_DIGEST_LENGTH 20

typedef struct {
   uint8_t data[64];
   uint32_t datalen;
   uint64_t bitlen;
   uint32_t state[5];
} SHA1_CTX;

void SHA1_Init(SHA1_CTX *ctx);
void SHA1_Update(SHA1_CTX *ctx, const uint8_t *data, size_t len);
void SHA1_Final(SHA1_CTX *ctx, uint8_t *md);
uint8_t* SHA1(const uint8_t* buf, size_t n, uint8_t* md);

#endif // SHA1_H
