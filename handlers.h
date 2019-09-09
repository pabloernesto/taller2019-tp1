#ifndef HANDLERS_H_
#define HANDLERS_H_

// context is not used by client handlers, and should receive NULL
// On server handlers, context receives a pointer to the active sudoku board
typedef void Handler(int connection, void *context);

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
