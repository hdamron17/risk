/* Copyright 2019 - Hunter Damron */

#include "game.h"
#include "game_initialization.h"
#include "graphical_mode.h"
#include "terminal_mode.h"

int main() {
  bool graphical = false;
  CountriesMap* map = load_map("assets/simple");
  if (graphical)
    graphical_main(map);
  else
    terminal_main(map);
  rmCountriesMap(map);
}
