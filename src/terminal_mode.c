/* Copyright 2019 Hunter Damron */

#include "terminal_mode.h"

#define max_players 6  // Depends on number of colors

bool confirm_quit() {
  char* quit = readline("Would you like to quit? [y/N]\n>> ");
  if (!quit)
    return true;
  if (quit[0] == 'Y' || quit[0] == 'y') {
    return true;
  }
  return false;
}

unsigned int get_nplayers() {
  char* nplayers_prompt = "Enter the number of players:\n>> ";
  bool valid_nplayers = false;
  unsigned int nplayers;
  do {
    char* nplayers_str = readline(nplayers_prompt);
    if (!nplayers_str) {
      free(nplayers_str);
      if (confirm_quit())
        return 1;
      continue;
    }
    nplayers = atoi(nplayers_str);
    if (!nplayers || nplayers > max_players) {
      err("Invalid number of players \'%s\' (max is %d)\n", nplayers_str, max_players);
      free(nplayers_str);
      continue;
    }
    valid_nplayers = true;
    free(nplayers_str);
  } while (!valid_nplayers);
  return nplayers;
}

typedef enum {SUCCESS, FAILURE, QUIT, DONE} RET_VAL;

RET_VAL list_terretories_cmd(CountriesMap* map, size_t player UNUSED, char** argv, size_t argc) {
  if (argc > 1) {
    for (size_t i = 1; i < argc; ++i) {
      debug("ls %s\n", argv[i]);
      size_t terretory_index = lookup_country(map, argv[i]);
      if (terretory_index != map->ncountries) {
        list_connections(map, terretory_index);
      } else {
        warn("Unknown country \'%s\'\n", argv[i]);
      }
    }
  } else {
    list_terretories(map);
  }
  return SUCCESS;
}

RET_VAL quit_cmd(CountriesMap* map UNUSED, size_t player UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  if (confirm_quit())
    return QUIT;
  else
    return SUCCESS;
}

RET_VAL done_cmd(CountriesMap* map UNUSED, size_t player UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  return DONE;
}

RET_VAL player_cmd(CountriesMap* map UNUSED, size_t player, char** argv UNUSED, size_t argc UNUSED) {
  #if COLOR
  printf("\x1b[%lumYou are player %lu\x1b[0m\n", BASE_COLOR+player, player+1);
  #else
  printf("You are player %lu\n", player+1);
  #endif
  return SUCCESS;
}

RET_VAL nop(CountriesMap* map UNUSED, size_t player UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  return SUCCESS;
}

typedef struct {
  const char* str;
  RET_VAL (*cmd_fn) (CountriesMap*, size_t, char**, size_t);
} command;

const command commands[] = {
  {"ls", list_terretories_cmd},
  {"quit", quit_cmd},
  {"done", done_cmd},
  {"player", player_cmd},
  {"", nop}  // nop must be last
};

RET_VAL terminal_loop(CountriesMap* map, size_t player) {
  char* cmd_str;
  bool done = false;
  RET_VAL ret;
  do {
    cmd_str = readline(">> ");
    if (!cmd_str) {
      if (confirm_quit())
        return 1;
      continue;
    }
    size_t num_tokens = token_count(cmd_str, ' ');
    char* tokenized_cmd[num_tokens];
    for (size_t i = 0; i < num_tokens; ++i) {
      char* token = strsep(&cmd_str, " ");
      tokenized_cmd[i] = malloc(strlen(token) + 1);
      strcpy(tokenized_cmd[i], token);
    }
    for (size_t i = 0; strcmp(commands[i].str, ""); ++i) {
      if (!strcmp(tokenized_cmd[0], commands[i].str)) {
        debug("Matched command %s\n", commands[i].str);
        ret = commands[i].cmd_fn(map, player, tokenized_cmd, num_tokens);
        break;
      }
    }

    free(cmd_str);
    done = true;  // TODO make a quit option
  } while (ret == SUCCESS);  // All other options constitute quiting
  return ret;
}

int terminal_main(CountriesMap* map) {
  unsigned int nplayers = get_nplayers();
  if (!nplayers) return 1;
  randomize_ownership(map, nplayers);
  RET_VAL (*player_loop_fn[nplayers]) (CountriesMap*, size_t);
  for (size_t i = 0; i < nplayers; ++i) {
    player_loop_fn[i] = terminal_loop;  // TODO add in automated player
  }
  RET_VAL ret = DONE;
  size_t turn = 0;
  while (ret == DONE) {
    ret = (*player_loop_fn[turn])(map, turn);
    turn = (turn + 1) % nplayers;
  }
  return (int) (ret == FAILURE);
}
