/* Copyright 2019 Hunter Damron */

#include "tests.h"
#include "util.c"

void test_token_count() {
  int x = token_count("s c t", ' ');
  int y = token_count(" s c t", ' ');
  int z = token_count("  s  c    t                 ", ' ');
  int w = token_count(" s c    t ", ' ');
  ASSERT(x == y && y == z && z == w && w == 3, "All should be 3\n");
  ASSERT(token_count("   sa c  sdfs  ts ", ' ') == 4, "Should be 4\n");
  ASSERT(token_count("ls ", ' ') == 1, "Should be 1\n");
}

int main() {
  test_token_count();
  return 0;
}
