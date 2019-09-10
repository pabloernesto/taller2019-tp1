#include "message.h"

#include "socket.h"
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

const char *Message_Get(int connection) {
  static char buffer[1024];
  if (Socket_ReceiveN(connection, sizeof(__uint32_t), buffer) < 4)
    return NULL;

  const int len = ntohl(*(__uint32_t*)buffer);
  if (len > 1024) return NULL;

  memset(buffer, 0, sizeof(buffer));
  if (Socket_ReceiveN(connection, len, buffer) < len)
    return NULL;
  
  return buffer;
}
