#ifndef HANDLERS_H_
#define HANDLERS_H_

typedef void Handler(int connection);

// Query the table given a certain command
Handler *get_handler(const char *key);


// Initialize the table of command handlers
void server_hash_handlers();

// Print an error and terminate the program
void server_handle_default(int connection);


// Initialize the table of command handlers
void client_hash_handlers();

// Print an error and terminate the program
void client_handle_default(int connection);

#endif //HANDLERS_H_
