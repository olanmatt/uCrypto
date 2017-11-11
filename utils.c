#include <stdint.h>
#include "utils.h"
#include <string>

uint32_t rotl32(uint32_t x, uint32_t c)
{
   return (((x) << (c)) | ((x) >> (32 - (c))));
}

uint32_t rotr32(uint32_t x, uint32_t c)
{
   return (((x) >> (c)) | ((x) << (32 - (c))));
}

uint32_t byteswap32(uint32_t n)
{
   return ((n>>24)&0xff) | // move byte 3 to byte 0
      ((n<<8)&0xff0000) | // move byte 1 to byte 2
      ((n>>8)&0xff00) | // move byte 2 to byte 1
      ((n<<24)&0xff000000); // byte 0 to byte 3
}

uint64_t byteswap64(uint64_t n)
{
   return ( (((n) >> 56) & 0x00000000000000FF) | (((n) >> 40) & 0x000000000000FF00) | \
         (((n) >> 24) & 0x0000000000FF0000) | (((n) >>  8) & 0x00000000FF000000) | \
         (((n) <<  8) & 0x000000FF00000000) | (((n) << 24) & 0x0000FF0000000000) | \
         (((n) << 40) & 0x00FF000000000000) | (((n) << 56) & 0xFF00000000000000) );
}

void hexstring2bytearray(const std::string str, uint8_t* res)
{
   for (int i = 0; i < str.size() / 2; i++) {
      res[i] = strtol(str.substr(i * 2, 2).c_str(), NULL, 16);
   }
}

/*
void print_digest(const uint8_t *buf, size_t n) {
   for (size_t i = 0; i < n; ++i) {
      printf("%02x ", buf[i]);
   }
}

// generates a deterministic buffer of n bytes
// http://www.learncpp.com/cpp-tutorial/59-random-number-generation/
uint8_t* generate_bytes(size_t n) {
	uint8_t *res = (uint8_t*)malloc(n);
	
	// our initial starting seed is 5323
	uint64_t seed = 5323;

	for (size_t i = 0; i < n; i++) {
		// Take the current seed and generate a new value from it
		// Due to our use of large constants and overflow, it would be
		// very hard for someone to predict what the next number is
		// going to be from the previous one.
		seed = (8253729 * seed + 2396403); 
		// Take the seed and return a value between 0 and 32767
		res[i] = seed % (UINT8_MAX + 1);
	}

	return res;
}
*/
