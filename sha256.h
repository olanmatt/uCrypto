#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stdlib.h>

#define SHA256_DIGEST_LENGTH 32

void sha256(const uint8_t* buf, size_t n, uint8_t* res);

#endif // SHA256_H
