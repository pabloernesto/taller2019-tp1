#include "handlers.h"

#include <search.h>
#include <stdio.h>
#include "socket.h"
#include <stdlib.h>
#include <string.h>
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
  char buffer1[5] = "P...";

  const char *s = strtok(NULL, " ,\n");   // get cell
  if (!s || !strchr("0123456789", *s)) goto error;
  buffer1[3] = *s - '0';

  s = strtok(NULL, " ,\n");   // ignore the "in" in the put statement
  if (!s) goto error;

  s = strtok(NULL, " ,\n");   // get row
  if (!s || !strchr("0123456789", *s)) goto error;
  buffer1[1] = *s - '0';

  s = strtok(NULL, " ,\n");   // get col
  if (!s || !strchr("0123456789", *s)) goto error;
  buffer1[2] = *s - '0';

  Socket_SendN(connection, 4, buffer1);

  const char *msg = Message_Get(connection);
  fputs(msg, stdout);
  return;

error:
  fputs("client handle_put: malformed put statement\n", stderr);
  exit(1);
}

void client_handle_default(int connection) {
  fputs("client: bad first token\n", stderr);
  Socket_Close(connection);
  hdestroy();
  exit(1);
}
