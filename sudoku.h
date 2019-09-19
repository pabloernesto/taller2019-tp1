#ifndef SUDOKU_H_
#define SUDOKU_H_

// typedef'ed for user convenience
// Board state is represented with the chars '1' through '9',
//   and the '0' character to mean "undefined"
typedef char SudokuBoard[9][9];

struct Sudoku {
  SudokuBoard board;
  SudokuBoard hints;
};

// Checks if the board breaks some rule.
// Returns 1 if ok, 0 if not, or a negative number if an error ocurred.
int Sudoku_Verify(const struct Sudoku *game);

// Sets all squares on the board to undefined ('0')
void SudokuBoard_Clear(SudokuBoard board);

// Set the game board to it's initial state.
void Sudoku_Reset(struct Sudoku* game);

// Update the board state.
// returns 0 on success, or -1 if the cell was already occupied by a hint.
// Passing a row/col outside of the range 0-8 is a checked runtime error.
// Passing a val outside of the range 1-9 is a checked runtime error.
int Sudoku_Put(struct Sudoku *game, int row, int col, int val);

// Pretty-print the sudoku board
// Returns a pointer to a static memory segment, do not free
const char *Sudoku_Pretty(const struct Sudoku *game);

#endif  //SUDOKU_H_
