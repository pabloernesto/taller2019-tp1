#ifndef MESSAGE_H_
#define MESSAGE_H_

const char *Message_Get(int connection);
void Message_Send(int connection, const char *msg);

#endif  // MESSAGE_H_
