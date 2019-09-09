#define _DEFAULT_SOURCE
#include "client_handlers.h"

#include <stdio.h>
#include "socket.h"
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
  if (argc < 2) return 1;

  if (!strcmp(argv[1], "server")) {
    if (argc != 3) return 1;
  
    const char *port = argv[2];
    int listener = Socket_Listen(port);
    int connection = Socket_Accept(listener);
    Socket_Close(listener);   // accept a single connection

    char get;
    Socket_ReceiveN(connection, 1, &get);
    __uint32_t msg_size = htonl(722);
    Socket_SendN(connection, 4, (char*) &msg_size);
    char msg[] = "U===========U===========U===========U\n"
                 "U 3 |   | 5 U   |   | 8 U   | 1 |   U\n"
                 "U---+---+---U---+---+---U---+---+---U\n"
                 "U   |   | 7 U   |   |   U 5 |   | 8 U\n"
                 "U---+---+---U---+---+---U---+---+---U\n"
                 "U 1 | 2 |   U 7 | 5 |   U   | 9 |   U\n"
                 "U===========U===========U===========U\n"
                 "U   |   | 9 U   | 7 |   U   |   | 4 U\n"
                 "U---+---+---U---+---+---U---+---+---U\n"
                 "U   |   | 4 U 3 |   | 5 U 9 |   |   U\n"
                 "U---+---+---U---+---+---U---+---+---U\n"
                 "U 7 |   |   U   | 9 |   U 8 |   |   U\n"
                 "U===========U===========U===========U\n"
                 "U   | 3 |   U   | 4 | 6 U   | 5 | 7 U\n"
                 "U---+---+---U---+---+---U---+---+---U\n"
                 "U 4 |   | 6 U   |   |   U 1 |   |   U\n"
                 "U---+---+---U---+---+---U---+---+---U\n"
                 "U   | 7 |   U 5 |   |   U 6 |   | 9 U\n"
                 "U===========U===========U===========U\n";
    // load buffer
    Socket_SendN(connection, 722, msg);

  } else if (!strcmp(argv[1], "client")) {
    if (argc != 4) return 1;

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
  } else {
    return 1;
  }
}
