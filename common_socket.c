#define _POSIX_C_SOURCE 201909
#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define BACKLOG 10

int Socket_Listen(const char *port) {
  struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_flags = AI_PASSIVE
  };
  struct addrinfo *addr;
  if (getaddrinfo(NULL, port, &hints, &addr))
    return -1;
  assert(addr);

  int listener;
  struct addrinfo *addr_p;
  for (addr_p = addr; addr_p; addr_p = addr_p->ai_next) {
    listener = socket(addr_p->ai_family, addr_p->ai_socktype,
      addr_p->ai_protocol);
    if (listener == -1)
      continue;

    if (!bind(listener, addr_p->ai_addr, addr_p->ai_addrlen))
      break;
    perror("bind");
    close(listener);
  }
  
  freeaddrinfo(addr);
  // No address succeeded
  if (!addr_p)
    return -2;

  if (listen(listener, BACKLOG) < 0)
    perror("listen");
  return listener;
}

int Socket_Accept(int listener) {
  socklen_t addr_len;
  struct sockaddr addr_p;
  return accept(listener, &addr_p, &addr_len);
}

int Socket_ReceiveN(int socket, int n, char *s) {
  int received = 0;
  while (received < n) {
    int r = recv(socket, s, n, 0);
    if (r < 0)
      return received;
    received += r;
  }
  return received;
}

void Socket_Close(int socket) {
  shutdown(socket, SHUT_RDWR);
  close(socket);
}
