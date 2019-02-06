/* Copyright 2019 Hunter Damron */

#ifndef TESTS_H
#define TESTS_H

#include <stdlib.h>
#include "util.h"

#define FAIL(msg) { err(msg); printf("\n"); exit(EXIT_FAILURE); }
// #define ASSERT(x, msg) { if(!(x)) { FAIL(msg); } }
#define ASSERT(x, msg) { message(,stdout,ASSERT,#x "\n"); if(!(x)) { FAIL(msg); } else { INFO("PASSED\n"); } }
#define INFO(...) message(,stdout,INFO,__VA_ARGS__)

#endif /* TESTS_H */
