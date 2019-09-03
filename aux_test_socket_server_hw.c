#include "socket.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  char *port = argv[1] ? argv[1] : "1234";
  int listener = Socket_Listen(port);
  switch (listener) {
    case -1: return 1;
    case -2: return 2;
  }
  int connection = Socket_Accept(listener);
  if (connection < 0) {
    perror("accept() failed");
    Socket_Close(listener);
    return 3;
  }

  char s[256] = { 0 };
  Socket_ReceiveN(connection, strlen("hello world!"), s);

  Socket_Close(connection);
  Socket_Close(listener);
  puts(s);
}
