#define _DEFAULT_SOURCE
#include "client_handlers.h"

#include <stdio.h>
#include "socket.h"
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 4)
    return 1;

  if (strcmp(argv[1], "client"))
    return 1;

  const char *host = argv[2];
  const char *port = argv[3];
  int connection = Socket_Connect(host, port);

hash_handlers();    // initialize handler table
  while (1) {
    // read a line of input
    char linebuffer[256];
    if (!fgets(linebuffer, sizeof(linebuffer), stdin))
      handle_default(connection);   // reached EOF

    // strsep the first token of the line, and query the handler table
    char *buffer_idx = linebuffer;
    Handler *f = get_handler(strsep(&buffer_idx, " \n"));
    f(connection);
  }
}
