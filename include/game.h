/* Copyright 2019 - Hunter Damron */

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game_structures.h"
#include "util.h"

#define BASE_COLOR 31

void randomize_ownership(CountriesMap* map, size_t nplayers);
unsigned int continent_bonus(const CountriesMap* map, const size_t player);  // TODO
unsigned int num_troops(const CountriesMap* map, const size_t player);
unsigned int start_troops(const CountriesMap* map, const size_t player);
void print_terretory(const CountriesMap* map, const size_t i);
void list_terretories(const CountriesMap* map);
void list_connections(const CountriesMap* map, const size_t i);
size_t lookup_country(const CountriesMap* map, const char* name);
bool place_troops(CountriesMap* map, TurnData* turn_data, const size_t i, const unsigned int num);

#endif /* GAME_H */
