#ifndef SOCKET_H_
#define SOCKET_H_

// Connect to a listening server
int Socket_Connect(const char *host, const char *port);

// Listen for incoming connections
int Socket_Listen(const char *port);
int Socket_Accept(int listener);

// Send or receive data, retrying until all data is sent
// Returns the number of bytes sent. If this is not n, it indicates
//   socket shutdown or some unrecoverable error.
int Socket_ReceiveN(int socket, int n, char *s);
int Socket_SendN(int socket, int n, const char *s);

// Shutdown the socket for read, write, or both, depending on the value of how
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
void Socket_Shutdown(int socket, int how);

#endif  //SOCKET_H_
