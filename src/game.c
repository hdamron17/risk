/* Copyright 2019 - Hunter Damron */

#include "game.h"

void randomize_ownership(CountriesMap* map, size_t nplayers) {
  size_t n = map->ncountries;
  size_t totals[nplayers];
  size_t player_max = n / nplayers;
  size_t cutoff = n - n % nplayers;  // After this distribution can be uneven
  for (size_t i = 0; i < nplayers; ++i)
    totals[i] = 0;
  srand(time(NULL));
  for (size_t i = 0; i < n; ++i) {
    size_t assign;
    do {
      assign = rand() % nplayers;
    } while ((i < cutoff && totals[assign] >= player_max)
            || totals[assign] >= player_max + 1);
    debug("Assigning %s to player %lu\n", map->countries[i]->name, assign);
    map->ownership[i] = assign;
    ++totals[assign];
  }
  for (size_t i = 0; i < nplayers; ++i)
    debug("Player %lu assigned %lu\n", i, totals[i]);
}
