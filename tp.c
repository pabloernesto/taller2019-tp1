#define _DEFAULT_SOURCE

#include <stdio.h>
#include "socket.h"
#include <stdlib.h>
#include <string.h>
#include <search.h>

typedef void Handler(int);
static void handle_get(int connection);
static void handle_exit(int connection);
static void handle_default(int connection);

int main(int argc, char **argv) {
  if (argc != 3)
    return 1;

  const char *host = argv[1];
  const char *port = argv[2];
  int connection = Socket_Connect(host, port);

  hcreate(32);
  Handler *handlers[] = {
    handle_get,
    handle_exit
  };
  hsearch((ENTRY){ .key="get" }, ENTER)->data = handlers;
  hsearch((ENTRY){ .key="exit" }, ENTER)->data = handlers + 1;

  while (1) {
    char linebuffer[256];
    char *ret_fgets = fgets(linebuffer, sizeof(linebuffer), stdin);
    if (!ret_fgets)
      handle_default(connection);
    //switch on first token
    char *buffer_idx = linebuffer;
    ENTRY *ret_hsearch = hsearch((ENTRY){ .key=strsep(&buffer_idx, " ,\n") }, FIND);
    if (!ret_hsearch)
      handle_default(connection);
    void (*f)(int) = *((Handler**)ret_hsearch->data);
    f(connection);
  }
  hdestroy();
}

static void handle_get(int connection) {
  Socket_SendN(connection, 1, "G");
  char buffer[722+5] = { 0 };
  Socket_ReceiveN(connection, 722+4, buffer);
  puts(buffer +4);
}

static void handle_exit(int connection) {
  Socket_Close(connection);
  exit(0);
}

static void handle_default(int connection) {
  fputs("client: bad first token\n", stderr);
  Socket_Close(connection);
  exit(1);
}
