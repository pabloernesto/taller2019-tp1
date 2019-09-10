#define _DEFAULT_SOURCE
#include "handlers.h"

#include <stdio.h>
#include "socket.h"
#include <string.h>
#include "sudoku.h"

static int server(const char *port);
static int client(const char *host, const char *port);
static int sudoku_from_file(struct Sudoku *s);

int main(int argc, char **argv) {
  if (argc < 2) return 1;

  if (!strcmp(argv[1], "server")) {
    if (argc != 3) return 1;
    return server(argv[2]);

  } else if (!strcmp(argv[1], "client")) {
    if (argc != 4) return 1;
    return client(argv[2], argv[3]);

  } else {
    return 1;
  }
}

static int server(const char *port) {
    struct Sudoku sudoku;
    if (sudoku_from_file(&sudoku)) {
      fputs("server: could not load board.txt\n", stderr);
      return 1;
    }
    int listener = Socket_Listen(port);
    int connection = Socket_Accept(listener);
    Socket_Close(listener);   // accept a single connection

    server_hash_handlers();
    char opcode[2] = { 0 };
    while (Socket_ReceiveN(connection, 1, opcode) == 1) {
      Handler *f = get_handler(opcode);
      if (f)
        f(connection, &sudoku);
      else
        server_handle_default(connection);
    }

    return 0;
}

static int client(const char *host, const char *port) {
    int connection = Socket_Connect(host, port);

    client_hash_handlers();    // initialize handler table
    while (1) {
      // read a line of input
      char linebuffer[256];
      if (!fgets(linebuffer, sizeof(linebuffer), stdin))
        client_handle_default(connection);   // reached EOF

      // strsep the first token of the line, and query the handler table
      char *buffer_idx = linebuffer;
      Handler *f = get_handler(strsep(&buffer_idx, " \n"));
      if (f)
        f(connection, NULL);
      else
        client_handle_default(connection);
    }

    return 0;
}

static int sudoku_from_file(struct Sudoku *s) {
  FILE *f = fopen("board.txt", "r");
  if (!f) return -1;
  SudokuBoard_Clear(s->board);

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!fscanf(f, " %1[0-9]", &(s->hints[i][j]))) {
        fclose(f);
        return -1;
      }
    }
  }
  fclose(f);
  return 0;
}
