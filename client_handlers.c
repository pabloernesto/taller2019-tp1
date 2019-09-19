#include "handlers.h"

#include <search.h>
#include <stdio.h>
#include "socket.h"
#include <stdlib.h>
#include "message.h"

static void handle_get(int connection, void *context);
static void handle_exit(int connection, void *context);
static void handle_reset(int connection, void *context);
static void handle_put(int connection, void *context);

static Handler *handlers[] = {
  handle_get,
  handle_exit,
  handle_reset,
  handle_put
};

void client_hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="get", .data=handlers }, ENTER);
  hsearch((ENTRY){ .key="exit", .data=handlers + 1 }, ENTER);
  hsearch((ENTRY){ .key="reset", .data=handlers + 2 }, ENTER);
  hsearch((ENTRY){ .key="put", .data=handlers + 3 }, ENTER);
}

static void handle_get(int connection, void *context) {
  Socket_SendN(connection, 1, "G");
  const char *msg = Message_Get(connection);
  fputs(msg, stdout);
}

static void handle_reset(int connection, void *context) {
  Socket_SendN(connection, 1, "R");
  const char *msg = Message_Get(connection);
  fputs(msg, stdout);
}

static void handle_exit(int connection, void *context) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}

static void handle_put(int connection, void *context) {
  char buffer[5] = "P...";

  int val, row, col;
  const int matched = sscanf((char*) context,
    "%d in %d,%d", &val, &row, &col);
  if (matched != 3) goto error;

  buffer[1] = row;
  buffer[2] = col;
  buffer[3] = val;

  Socket_SendN(connection, 4, buffer);

  const char *msg = Message_Get(connection);
  if (*msg == 'U')
    fputs(msg, stdout);
  else
    fputs(msg, stderr);

  return;

error:
  fputs("client handle_put: malformed put statement\n", stderr);
  exit(1);
}

void client_handle_default(int connection) {
  Socket_Close(connection);
  hdestroy();
  exit(1);
}
