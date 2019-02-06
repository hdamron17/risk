/* Copyright 2019 Hunter Damron */

#include "util.h"

size_t token_count(const char* s, const char c) {
  size_t len = strlen(s);
  size_t count = 0;
  bool in_delim = true;
  for (size_t i = 0; i < len; ++i) {
    if (s[i] == c) {
      in_delim = true;
    } else {
      if (in_delim) {
        count++;
      }
      in_delim = false;
    }
  }
  return count;
}

size_t int_length(const size_t num) {
  return (num == 0) ? 1 : (size_t) floor(log10((float)num)) + 1;
}
