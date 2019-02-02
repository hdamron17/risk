/* Copyright 2019 - Hunter Damron */

#include "graphical_mode.h"

int minw = 180, minh = 40;
int w = 0, h = 0;

bool window_check() {
  int h, w;
  getmaxyx(stdscr, h, w);
  if (h >= minh && w >= minw)
    return true;
  endwin();
  printf("RISK requires at least %d x %d character screen.\n"
      "Your screen is currently %d x %d\n", minw, minh, w, h);
  return false;
}

int graphical_main() {
  initscr();
  if (!window_check()) {
    return -1;
  }
  for (int i = 0; i < 0; ++i)
    getch();
  endwin();
  return 0;
}
