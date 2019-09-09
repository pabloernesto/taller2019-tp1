#define _DEFAULT_SOURCE
#include "handlers.h"

#include <stdio.h>
#include "socket.h"
#include <string.h>

int main(int argc, char **argv) {
  if (argc < 2) return 1;

  if (!strcmp(argv[1], "server")) {
    if (argc != 3) return 1;
  
    const char *port = argv[2];
    int listener = Socket_Listen(port);
    int connection = Socket_Accept(listener);
    Socket_Close(listener);   // accept a single connection

    server_hash_handlers();
    char opcode[2] = { 0 };
    while (Socket_ReceiveN(connection, 1, opcode) == 1) {
      Handler *f = get_handler(opcode);
      if (f)
        f(connection);
      else
        server_handle_default(connection);
    }

  } else if (!strcmp(argv[1], "client")) {
    if (argc != 4) return 1;

    const char *host = argv[2];
    const char *port = argv[3];
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
        f(connection);
      else
        client_handle_default(connection);
    }
  } else {
    return 1;
  }
}
