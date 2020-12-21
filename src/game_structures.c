/* Copyright 2019 Hunter Damron */

#include "game_structures.h"

Country* mkCountry(const char* name) {
  return mkCountryn(name, strlen(name));
}

Country* mkCountryn(const char* name, const size_t name_len) {
  Country* c = malloc(sizeof(Country));
  c->name = malloc(name_len+1);
  strncpy(c->name, name, name_len);
  c->troops = 1;
  return c;
}

void rmCountry(Country* c) {
  if (c) {
    free(c->name);
    free(c);
  }
}

CountriesMap* emptyCountiesMap(const size_t n) {
  CountriesMap* map = malloc(sizeof(CountriesMap));
  map->ncountries = n;
  map->countries = calloc(n, sizeof(Country*));
  map->connections = calloc(n * n, sizeof(bool));
  map->ownership = calloc(n, sizeof(unsigned int));
  return map;
}

void rmCountriesMap(CountriesMap* map) {
  for (size_t i = 0; i < map->ncountries; ++i) {
    if (map->countries[i]) {
      free(map->countries[i]);
    }
  }
  free(map->connections);
  free(map->ownership);
  free(map);
}

bool* conn(const CountriesMap* map, size_t i, size_t j) {
  return &map->connections[i * map->ncountries + j];
}

bool get_conn(const CountriesMap* map, size_t i, size_t j) {
  return *conn(map, i, j);
}
