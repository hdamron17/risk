/* Copyright 2019 Hunter Damron */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#ifndef DEBUG
#define DEBUG 0
#endif /* DEBUG */

#define max(x,y) ((x > y) ? x : y)

#define message(prefix,stream,tag,...) prefix { fprintf(stream, "[" #tag "] " __FILE__ ":%d  ", __LINE__); printf(__VA_ARGS__); }
#define debug(...) message(if (DEBUG),stdout,DBG,__VA_ARGS__)
#define warn(...) message(,stderr,WARN,__VA_ARGS__)
#define err(...) message(,stderr,ERROR,__VA_ARGS__)

#endif /* UTIL_H */