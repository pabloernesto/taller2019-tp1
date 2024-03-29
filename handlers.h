#ifndef HANDLERS_H_
#define HANDLERS_H_

// On client handlers, context points to the input line, sans the selector token
// On server handlers, context points to the active sudoku board
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
