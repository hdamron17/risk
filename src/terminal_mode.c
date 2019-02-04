/* Copyright 2019 Hunter Damron */

#include "terminal_mode.h"

#define max_players 6  // Depends on number of colors
#define ls_columns 80  // TODO possibly replace this with termcap/terminfo
#define base_color 31

bool confirm_quit() {
  char* quit = readline("Would you like to quit? [y/N]\n>> ");
  if (!quit)
    return true;
  if (quit[0] == 'Y' || quit[0] == 'y') {
    return true;
  }
  return false;
}

size_t int_length(size_t num) {
  return (num == 0) ? 1 : (size_t) floor(log10((float)num)) + 1;
}

void list_terretories(CountriesMap* map) {
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
    unsigned int color_start = base_color + map->ownership[i];
    printf("\x1b[%dm%d %s %d\x1b[0m   %s", color_start, map->ownership[i], map->countries[i]->name, map->countries[i]->troops, spaces);
    spaces[num_spaces] = ' ';
    if ((i + 1) % ncolumns == 0)
      printf("\n");
  }
  if (map->ncountries % ncolumns != 0) {
    printf("\n");  // Trailing newline if not already there
  }
}

int terminal_main(CountriesMap* map) {
  char* nplayers_prompt = "Enter the number of players:\n>> ";
  bool valid_nplayers = false;
  unsigned int nplayers;
  do {
    char* nplayers_str = readline(nplayers_prompt);
    if (!nplayers_str) {
      err("You must enter a number\n");
      free(nplayers_str);
      if (confirm_quit())
        return 1;
      continue;
    }
    nplayers = atoi(nplayers_str);
    if (!nplayers || nplayers > max_players) {
      err("Invalid number of players \'%s\' (max is %d)\n", nplayers_str, max_players);
      free(nplayers_str);
      if (confirm_quit())
        return 1;
      continue;
    }
    valid_nplayers = true;
    free(nplayers_str);
  } while (!valid_nplayers);
  randomize_ownership(map, nplayers);
  return 0;
}
