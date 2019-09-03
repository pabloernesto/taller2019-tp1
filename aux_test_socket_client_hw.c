#include "socket.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  char *port = argv[1] ? argv[1] : "1234";
  int connection = Socket_Connect("localhost", port);
  if (connection < 0)
    return -connection;

  Socket_SendN(connection, strlen("hello world!"), "hello world!");
  Socket_Close(connection);
}
