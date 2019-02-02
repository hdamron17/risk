/* Copyright 2019 Hunter Damron */

#include "game_structures.h"

Country* mkCountry(const char* name) {
  Country* c = malloc(sizeof(Country));
  size_t len = strlen(name);
  c->name = malloc(len+1);
  strncpy(c->name, name, len);
  return c;
}

void rmCountry(Country* c) {
  if (c) {
    free(c->name);
    free(c);
  }
}
