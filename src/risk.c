/* Copyright 2019 - Hunter Damron */

#include "game.h"
#include "graphical_mode.h"
#include "terminal_mode.h"

int main(int argc, char** argv) {
  bool graphical = false;
  if (graphical)
    graphical_main();
  else
    terminal_main();
}
