#define _DEFAULT_SOURCE
#include "handlers.h"

#include <stdio.h>
#include "socket.h"
#include <string.h>
#include "sudoku.h"

static int server(int argc, char **argv);
static int client(int argc, char **argv);
static int sudoku_from_file(struct Sudoku *s);

int main(int argc, char **argv) {
  if (!strcmp(argv[1], "server")) {
    return server(argc, argv);

  } else if (!strcmp(argv[1], "client")) {
    return client(argc, argv);

  } else {
    puts("Modo no soportado, el primer parámetro debe ser server o client");
  }
}

static int server(int argc, char **argv) {
    if (argc != 3) {
      puts("Uso: ./tp server <puerto>");
      return 0;
    }

    struct Sudoku sudoku;
    if (sudoku_from_file(&sudoku)) {
      fputs("server: could not load board.txt\n", stderr);
      return 1;
    }
    int listener = Socket_Listen(argv[2]);
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

static int client(int argc, char **argv) {
    if (argc != 4) {
      puts("Uso: ./tp client <host> <puerto>");
      return 0;
    }

    int connection = Socket_Connect(argv[2], argv[3]);

    client_hash_handlers();    // initialize handler table
    while (1) {
      // read a line of input
      char linebuffer[256];
      if (!fgets(linebuffer, sizeof(linebuffer), stdin))
        client_handle_default(connection);   // reached EOF

      char *lineidx = linebuffer;
      // strsep the first token of the line, and query the handler table
      Handler *f = get_handler(strsep(&lineidx, " \n"));
      if (f)
        f(connection, lineidx);
      else
        client_handle_default(connection);
    }

    return 0;
}

static int sudoku_from_file(struct Sudoku *s) {
  FILE *f = fopen("board.txt", "r");
  if (!f) return -1;

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!fscanf(f, " %1[0-9]", &(s->hints[i][j]))) {
        fclose(f);
        return -1;
      }
    }
  }
  fclose(f);

  Sudoku_Reset(s);
  return 0;
}
