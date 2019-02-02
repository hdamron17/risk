/* Copyright 2018 - Hunter Damron */

#ifndef GAME_STRUCTURES_H
#define GAME_STRUCTURES_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  char* name;
} Country;

Country* mkCountry(const char* name);
void rmCountry(Country* c);

typedef struct {
  Country* countries;
  size_t ncountries;
  bool** connections;  // Adjacency matrix of countries
} CountriesMap;

#endif /* GAME_STRUCTURES_H */
