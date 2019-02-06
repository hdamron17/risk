/* Copyright 2019 Hunter Damron */

#include "tests.h"
#include "util.c"

int main() {
  int x = token_count("s c t", ' ');
  int y = token_count(" s c t", ' ');
  int z = token_count("  s  c    t                 ", ' ');
  int w = token_count(" s c    t ", ' ');
  ASSERT(x == y && y == z && z == w && w == 3, "All should be 3\n");
  ASSERT(token_count("   sa c  sdfs  ts ", ' ') == 4, "Should be 4\n");
  return 0;
}
