#include "handlers.h"

#include "socket.h"
#include <stdlib.h>
#include <search.h>
#include <arpa/inet.h>

static void handle_get(int connection);

static Handler *handlers[] = {
  handle_get
};

void server_hash_handlers() {
  hcreate(32);
  hsearch((ENTRY){ .key="G", .data=handlers }, ENTER);
}

static void handle_get(int connection) {
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
  Socket_SendN(connection, 722, msg);
}

void server_handle_default(int connection) {
  Socket_Close(connection);
  hdestroy();
  exit(0);
}
