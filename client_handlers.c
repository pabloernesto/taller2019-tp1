#include "client_handlers.h"

#include <search.h>
#include <stdio.h>
#include "socket.h"
#include <stdlib.h>

static void handle_get(int connection);
static void handle_exit(int connection);

static Handler *handlers[] = {
  handle_get,
  handle_exit
};

void hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="get", .data=handlers }, ENTER);
  hsearch((ENTRY){ .key="exit", .data=handlers + 1 }, ENTER);
}

Handler *get_handler(const char *key) {
  ENTRY *ret = hsearch((ENTRY){ .key=(char*) key }, FIND);
  return ret ? *(Handler **)(ret->data) : handle_default;
}

static void handle_get(int connection) {
  Socket_SendN(connection, 1, "G");
  char buffer[722+5] = { 0 };
  Socket_ReceiveN(connection, 722+4, buffer);
  puts(buffer +4);
}

static void handle_exit(int connection) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}

void handle_default(int connection) {
  fputs("client: bad first token\n", stderr);
  Socket_Close(connection);
  hdestroy();
  exit(1);
}
