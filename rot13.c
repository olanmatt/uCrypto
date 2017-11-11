#include "rot13.h"

#include <stdio.h>

void rot13(const char* str, size_t len, char* ret)
{
   for (size_t i = 0; i < len; i++) {
      char c = str[i];
      if (c >= 'A' && c <= 'Z') {
         ret[i] = 'A' + (((c - 'A') + 13) % 26);
      } else if (c >= 'a' && c <= 'z') {
         ret[i] = 'a' + (((c - 'a') + 13) % 26);
      } else {
         ret[i] = c;
      }
   }
}
