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
  char* nplayers_prompt = "Enter the number of players: ";
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

RET_VAL list_terretories_cmd(CountriesMap* map, TurnData* player_data UNUSED, char** argv, size_t argc) {
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

RET_VAL quit_cmd(CountriesMap* map UNUSED, TurnData* player_data UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  if (confirm_quit())
    return QUIT;
  else
    return SUCCESS;
}

RET_VAL done_cmd(CountriesMap* map UNUSED, TurnData* player_data UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  return DONE;
}

RET_VAL player_cmd(CountriesMap* map UNUSED, TurnData* player_data, char** argv UNUSED, size_t argc UNUSED) {
  #if COLOR
  printf("\x1b[%lumYou are player %lu\x1b[0m\n", BASE_COLOR+player_data->player, player_data->player+1);
  #else
  printf("You are player %lu\n", player_data->player+1);
  #endif
  return SUCCESS;
}

RET_VAL place_cmd(CountriesMap* map, TurnData* turn_data, char** argv, size_t argc) {
  if (argc != 3) {
    printf("Usage: place <country> <number>\n");
    return FAILURE;
  }
  size_t i = lookup_country(map, argv[1]);  // TODO check != length
  unsigned int num = atoi(argv[2]);  // TODO check valid number
  bool ret = place_troops(map, turn_data, i, num);
  return (ret) ? SUCCESS : FAILURE;
}

RET_VAL num_troops_cmd(CountriesMap* map UNUSED, TurnData* turn_data, char** argv UNUSED, size_t argc UNUSED) {
  printf("You have %u troops\n", turn_data->unplaced_troops);
  return SUCCESS;
}

RET_VAL attack_cmd(CountriesMap* map, TurnData* turn_data, char** argv, size_t argc) {
    // TODO
    err("Command 'attack' unimplemented\n");
    return FAILURE;
}

RET_VAL nop(CountriesMap* map UNUSED, TurnData* player_data UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  return SUCCESS;
}

typedef struct {
  const char* str;
  RET_VAL (*cmd_fn) (CountriesMap*, TurnData*, char**, size_t);
  const char* help_str;
} command;

const command commands[];  // Forward declare for help command
RET_VAL help_cmd(CountriesMap* map UNUSED, TurnData* player_data UNUSED, char** argv UNUSED, size_t argc UNUSED) {
  for (size_t i = 0; strcmp(commands[i].str, ""); ++i) {
    puts(commands[i].help_str);
  }
  return SUCCESS;
}

const command commands[] = {
  {"ls", list_terretories_cmd, "ls [country..]   Lists all countries or connections to specified countries"},
  {"quit", quit_cmd,           "quit             Quits the current game"},
  {"done", done_cmd,           "done             Ends the current turn"},
  {"player", player_cmd,       "player           Tells which player is currently taking their turn"},
  {"help", help_cmd,           "help             Displays this help menu"},
  {"place", place_cmd,         "place            Place troops on a country, usage: place <country> <num>"},
  {"troops", num_troops_cmd,   "troops           Returns the number of troops available to place"},
  {"attack", attack_cmd,       "attack           Attack one country from another, usage: attack <offender> <defender> <num>"},
  {"", nop, ""}  // nop must be last
};

RET_VAL terminal_loop(CountriesMap* map, size_t player) {
  char* cmd_str;
  bool done = false;
  RET_VAL ret = SUCCESS;
  TurnData player_data = {player, PLACEMENT, start_troops(map, player)};
  debug("Starting with %u troops\n", player_data.unplaced_troops);
  do {
    cmd_str = readline(">> ");
    char* cmd_str_backup = cmd_str;  // So we can still free after using strsep
    if (!cmd_str) {
      if (confirm_quit())
        return 1;
      continue;
    }
    size_t num_tokens = token_count(cmd_str, ' ');
    debug("Number of tokens in command: %lu\n", num_tokens);
    if (!num_tokens) {
      free(cmd_str_backup);
      continue;
    }
    add_history(cmd_str);
    char* tokenized_cmd[num_tokens];
    for (size_t i = 0; i < num_tokens; ++i) {
      char* token = strsep(&cmd_str, " ");
      tokenized_cmd[i] = malloc(strlen(token) + 1);
      strcpy(tokenized_cmd[i], token);
    }
    bool bad_command = true;
    for (size_t i = 0; strcmp(commands[i].str, ""); ++i) {
      if (!strcmp(tokenized_cmd[0], commands[i].str)) {
        debug("Matched command %s\n", commands[i].str);
        ret = commands[i].cmd_fn(map, &player_data, tokenized_cmd, num_tokens);
        bad_command = false;
        break;
      }
    }
    if (bad_command) {
      err("Unrecognized command \'%s\'\n", tokenized_cmd[0]);
    }
    free(cmd_str_backup);
    done = true;  // TODO make a quit option
  } while (ret != QUIT || ret != DONE);  // All other options constitute quiting
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
