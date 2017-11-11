#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <string>

uint32_t rotl32(uint32_t x, uint32_t c);
uint32_t rotr32(uint32_t x, uint32_t c);

uint32_t byteswap32(uint32_t n);
uint64_t byteswap64(uint64_t n);

void hexstring2bytearray(std::string str, uint8_t* res);

/*
void print_digest(const uint8_t *buf, size_t n);

// generates a deterministic buffer of n bytes
// http://www.learncpp.com/cpp-tutorial/59-random-number-generation/
uint8_t* generate_bytes(size_t n);
*/

#endif // UTILS_H
