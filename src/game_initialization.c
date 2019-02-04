/* Copyright 2019 Hunter Damron */

#include "game_initialization.h"

const char* terretories_file = "/terretories.txt";
const char* connections_file = "/connections.txt";

char* pathjoin(const char* dir, const char* f) {
  size_t len = strlen(dir) + strlen(f) + 1;
  char* path = malloc(len);
  strcpy(path, dir);
  strcat(path, f);
  path[len-1] = '\0';
  return path;
}

FILE* load_asset(const char* dir, const char* f, bool* failed) {
  char* fullpath = pathjoin(dir, f);
  FILE* openned = fopen(fullpath, "r");
  if (!openned) {
    err("Failed to open asset file %s\n", fullpath);
    if (failed) *failed = true;
  }
  free(fullpath);
  return openned;
}

void file_dimn(FILE* f, size_t bufflen, size_t* lines, size_t* max_len) {
  *max_len = 0;
  *lines = 0;
  char buff[bufflen];
  size_t n = 0,  // Number of read bytes
         stored = 0,  // Number of stored bytes before start start pointer
         start = 0;   // Start of current look
  long fpos = ftell(f);
  do {
    n = fread(buff, sizeof(char), bufflen, f);
    start = 0;
    for (size_t i = 0; i < n; ++i) {
      if (buff[i] == '\n') {
        size_t this_len = stored + i - start;
        debug("Line of length %lu = %lu + %lu - %lu\n", this_len, stored, i, start);
        *max_len = max(this_len, *max_len);
        start = i+1;
        stored = 0;
        if (this_len) ++*lines;
      }
    }
    stored = stored + n - start;
  } while (n > 0);
  size_t final_len = stored + n - start;
  *max_len = max(final_len, *max_len);
  if (final_len) ++*lines;
  fseek(f, fpos, SEEK_SET);
}

const int DEFAULT_BUFFLEN = 40;
void file_dim(FILE* f, size_t* lines, size_t* max_len) {
  file_dimn(f, DEFAULT_BUFFLEN, lines, max_len);
}

CountriesMap* load_terretories(const char* dir, bool* failed) {
  bool this_failed = false;
  FILE* f = load_asset(dir, terretories_file, &this_failed);
  if (this_failed) {
    if (failed) *failed = this_failed;  // Pass up the failure
    return NULL;
  }
  size_t n, max_len;
  file_dim(f, &n, &max_len);
  max_len++;  // To allow null terminator
  debug("terretories file dim: %lu x %lu\n", n, max_len);
  char buff[max_len];
  CountriesMap* map = emptyCountiesMap(n);
  map->ncountries = n;
  for (size_t i = 0; i < n; ++i) {
    fscanf(f, "%s", buff);
    map->countries[i] = mkCountry(buff);
    debug("terretory %lu has contents \'%s\'\n", i, map->countries[i]->name);
  }
  fclose(f);
  return map;
}

size_t get_index(CountriesMap* map, char* key) {
  size_t i;
  for (i = 0; i < map->ncountries; ++i) {
    if (!strcmp(key, map->countries[i]->name)) {
      return i;
    }
  }
  return i;  // Returns length ncountries if out of bounds
}

bool load_connections(const char* dir, CountriesMap* map) {
  bool failed = false;
  FILE* f = load_asset(dir, connections_file, &failed);
  if (failed) {
    return false;
  }
  size_t n, max_len;
  file_dim(f, &n, &max_len);
  char buff[max_len];
  for (size_t i = 0; i < n; ++i) {
    fscanf(f, "%[^\n]", buff);
    char* key = strtok(buff, " ");
    size_t key_index = get_index(map, key);
    if (key_index == map->ncountries) {
      err("Unknown key \'%s\'\n", key);
      return false;
    }
    debug("Connect %s [%lu]\n", key, key_index);
    char* dep = strtok(NULL, " ");
    while (dep) {
      size_t dep_index = get_index(map, dep);
      if (dep_index == map->ncountries) {
        err("Unknown connection \'%s\'\n", dep);
        return false;
      }
      debug("-- %s [%lu]\n", dep, dep_index);
      *conn(map, key_index, dep_index) = true;
      *conn(map, dep_index, key_index) = true;
      dep = strtok(NULL, " ");
    }
    fgetc(f);  // Get the newline off the end
  }
  fclose(f);
  return true;
}

CountriesMap* load_map(const char* dir) {
  bool failed = false;
  CountriesMap* map = load_terretories(dir, &failed);
  failed &= load_connections(dir, map);
  if (failed) {
    err("Failed to load map from directory \'%s\'\n", dir);
    return NULL;
  }
  return map;
}
