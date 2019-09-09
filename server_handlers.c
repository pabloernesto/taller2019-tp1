#include "handlers.h"

#include "sudoku.h"
#include "socket.h"
#include <stdlib.h>
#include <search.h>
#include <arpa/inet.h>
#include <string.h>

static void handle_get(int connection, void *context);

static Handler *handlers[] = {
  handle_get
};

void server_hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="G", .data=handlers }, ENTER);
}

static void handle_get(int connection, void *context) {
  struct Sudoku *game = context;

  // Pretty-print the board
  const char *msg = Sudoku_Pretty(game);
  int msg_len = strlen(msg);

  // Send the board
  __uint32_t msg_len_encoded = htonl(msg_len);
  Socket_SendN(connection, sizeof(msg_len_encoded), (char*) &msg_len_encoded);
  Socket_SendN(connection, msg_len, msg);
}

void server_handle_default(int connection) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}
