#include "handlers.h"

#include "sudoku.h"
#include "socket.h"
#include <stdlib.h>
#include <search.h>
#include <arpa/inet.h>
#include <string.h>
#include "message.h"

static void handle_get(int connection, void *context);
static void handle_reset(int connection, void *context);

static Handler *handlers[] = {
  handle_get,
  handle_reset
};

void server_hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="G", .data=handlers }, ENTER);
  hsearch((ENTRY){ .key="R", .data=handlers + 1 }, ENTER);
}

static void handle_get(int connection, void *context) {
  struct Sudoku *game = context;

  // Pretty-print the board
  Message_Send(connection, Sudoku_Pretty(game));
}

static void handle_reset(int connection, void *context) {
  struct Sudoku *game = context;
  SudokuBoard_Clear(game->board);

  // Pretty-print the board
  Message_Send(connection, Sudoku_Pretty(game));
}

void server_handle_default(int connection) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}
