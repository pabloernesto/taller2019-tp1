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
static void handle_put(int connection, void *context);

static Handler *handlers[] = {
  handle_get,
  handle_reset,
  handle_put
};

void server_hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="G", .data=handlers }, ENTER);
  hsearch((ENTRY){ .key="R", .data=handlers + 1 }, ENTER);
  hsearch((ENTRY){ .key="P", .data=handlers + 2 }, ENTER);
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

static void handle_put(int connection, void *context) {
  struct Sudoku *game = context;
  struct __attribute__((__packed__)) {
    uint8_t row, col, val;
  } input;
  Socket_ReceiveN(connection, 3, (char*) &input);

  if (input.row < 1 || input.row > 9
      || input.col < 1 || input.col > 9) {
    // error en los indices
    Message_Send(connection, "Error en los índices. Rango soportado: [1,9]\n");
    return;
  } else if (input.val < 1 || input.val > 9) {
    Message_Send(connection, "Error en el valor ingresado. Rango soportado: [1,9]\n");
    return;
  }
  input.row--; input.col--; input.val += '0';

  if (Sudoku_Put(game, input.row, input.col, input.val))
    Message_Send(connection, "La celda indicada no es modificable\n");
  else
    Message_Send(connection, Sudoku_Pretty(game));
}

void server_handle_default(int connection) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}
