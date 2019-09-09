#include "handlers.h"

#include <search.h>
#include <stdio.h>
#include "socket.h"
#include <stdlib.h>

static void handle_get(int connection, void *context);
static void handle_exit(int connection, void *context);
static void handle_reset(int connection, void *context);

static Handler *handlers[] = {
  handle_get,
  handle_exit,
  handle_reset
};

void client_hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="get", .data=handlers }, ENTER);
  hsearch((ENTRY){ .key="exit", .data=handlers + 1 }, ENTER);
  hsearch((ENTRY){ .key="reset", .data=handlers + 2 }, ENTER);
}

static void handle_get(int connection, void *context) {
  Socket_SendN(connection, 1, "G");
  char buffer[722+5] = { 0 };
  Socket_ReceiveN(connection, 722+4, buffer);
  puts(buffer +4);
}

static void handle_reset(int connection, void *context) {
  Socket_SendN(connection, 1, "R");
  char buffer[722+5] = { 0 };
  Socket_ReceiveN(connection, 722+4, buffer);
  puts(buffer +4);
}

static void handle_exit(int connection, void *context) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}

void client_handle_default(int connection) {
  fputs("client: bad first token\n", stderr);
  Socket_Close(connection);
  hdestroy();
  exit(1);
}
