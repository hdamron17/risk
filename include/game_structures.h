/* Copyright 2019 - Hunter Damron */

#ifndef GAME_STRUCTURES_H
#define GAME_STRUCTURES_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

typedef struct {
  char* name;
  unsigned int troops;
} Country;

Country* mkCountry(const char* name);
Country* mkCountryn(const char* name, const size_t name_len);
void rmCountry(Country* c);

typedef struct {
  Country** countries;  // List of Country pointers
  size_t ncountries;
  bool* connections;  // Adjacency matrix of countries
  unsigned int* ownership;  // Who owns what
} CountriesMap;

CountriesMap* emptyCountiesMap(const size_t n);
void rmCountriesMap(CountriesMap* map);
bool* conn(const CountriesMap* map, size_t i, size_t j);
bool get_conn(const CountriesMap* map, size_t i, size_t j);

typedef enum {PLACEMENT, ATTACK, MOVEMENT, CARDS} GameMode;

typedef struct {
  size_t player;
  GameMode mode;
  unsigned int unplaced_troops;
} TurnData;

#endif /* GAME_STRUCTURES_H */
