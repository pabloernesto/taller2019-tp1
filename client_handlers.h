#ifndef CLIENT_HANDLERS_H_
#define CLIENT_HANDLERS_H_

typedef void Handler(int);

// Initialize the table of command handlers
void hash_handlers();

// Query the table given a certain command
Handler *get_handler(const char *key);

// Print an error and terminate the program
void handle_default(int connection);

#endif //CLIENT_HANDLERS_H_
