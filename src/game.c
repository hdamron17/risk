/* Copyright 2019 - Hunter Damron */

#include "game.h"

#define ls_columns 80  // TODO possibly replace this with termcap/terminfo

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

unsigned int num_troops(const CountriesMap* map, const size_t player) {
  unsigned int n = 0;
  for (size_t i = 0; i < map->ncountries; ++i) {
    if (map->ownership[i] == player) {
      ++n;
    }
  }
  return n;
}

unsigned int continent_bonus(const CountriesMap* map, const size_t player) {
  warn("Function undefined");
  return 0;  // TODO add continent bonus
}

unsigned int start_troops(const CountriesMap* map, const size_t player) {
  unsigned int base = num_troops(map, player) / 3;
  return base + continent_bonus(map, player);
}

void print_terretory(const CountriesMap* map, const size_t i) {
  #if COLOR
  unsigned int color_start = BASE_COLOR + map->ownership[i];
  printf("\x1b[%dm%d %s %d\x1b[0m", color_start, map->ownership[i]+1, map->countries[i]->name, map->countries[i]->troops);
  #else
  printf("%d %s %d", map->ownership[i]+1, map->countries[i]->name, map->countries[i]->troops);
  #endif
}

void list_terretories(const CountriesMap* map) {
  size_t max_len = 0;
  size_t line_lengths[map->ncountries];
  for (size_t i = 0; i < map->ncountries; ++i) {
    size_t name_len = strlen(map->countries[i]->name);
    size_t troops_len = int_length(map->countries[i]->troops);
    size_t owner_len = int_length(map->ownership[i]);
    size_t this_len = name_len + troops_len + owner_len + 5;  // Three spaces after
    line_lengths[i] = this_len;
    max_len = max(this_len, max_len);
  }
  size_t ncolumns = ls_columns / max_len;
  debug("Max line length %lu -> %lu columns\n", max_len, ncolumns);
  char spaces[max_len];
  for (size_t i = 0; i < max_len; ++i)
    spaces[i] = ' ';
  for (size_t i = 0; i < map->ncountries; ++i) {
    size_t num_spaces = max_len - line_lengths[i];
    spaces[num_spaces] = '\0';
    print_terretory(map, i);
    printf("   %s", spaces);
    spaces[num_spaces] = ' ';
    if ((i + 1) % ncolumns == 0)
      printf("\n");
  }
  if (map->ncountries % ncolumns != 0) {
    printf("\n");  // Trailing newline if not already there
  }
}

void list_connections(const CountriesMap* map, const size_t i) {
  print_terretory(map, i);
  printf("\n");
  for (size_t j = 0; j < map->ncountries; ++j) {
    if (get_conn(map, i,j)) {
      printf("  ");
      print_terretory(map, j);
      printf("\n");
    }
  }
}

size_t lookup_country(const CountriesMap* map, const char* name) {
  size_t i;
  for (i = 0; i < map->ncountries; ++i) {
    if (!strcmp(name, map->countries[i]->name)) {
      return i;
    }
  }
  return i;
}
