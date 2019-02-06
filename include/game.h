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

enum game_mode {PLACEMENT, ATTACK, MOVEMENT, CARDS};

void randomize_ownership(CountriesMap* map, size_t nplayers);
unsigned int continent_bonus(const CountriesMap* map, const size_t player);  // TODO
unsigned int num_troops(const CountriesMap* map, const size_t player);
unsigned int start_troops(const CountriesMap* map, const size_t player);
void print_terretory(const CountriesMap* map, const size_t i);
void list_terretories(const CountriesMap* map);
void list_connections(const CountriesMap* map, const size_t i);
size_t lookup_country(const CountriesMap* map, const char* name);

#endif /* GAME_H */
