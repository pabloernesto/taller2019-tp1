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

// Checks if the board is solved.
// Returns 1 if solved, 0 if not, or a negative number if an error ocurred.
int Sudoku_IsSolved(const struct Sudoku *game);

// Sets all squares on the board to undefined ('0')
void SudokuBoard_Clear(SudokuBoard board);

// Pretty-print the sudoku board
// Returns a pointer to a static memory segment, do not free
const char *Sudoku_Pretty(const struct Sudoku *game);

#endif  //SUDOKU_H_
