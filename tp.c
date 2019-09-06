#include <stdio.h>
#include "socket.h"

int main(int argc, char **argv) {
  if (argc != 3)
    return 1;

  const char *host = argv[1];
  const char *port = argv[2];
  int connection = Socket_Connect(host, port);
  Socket_SendN(connection, 1, "G");
  char buffer[722+5] = { 0 };
  Socket_ReceiveN(connection, 722+4, buffer);
  Socket_Close(connection);
  puts(buffer +4);
}
