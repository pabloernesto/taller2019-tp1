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

// Sets all squares on the board to undefined ('0')
void SudokuBoard_Clear(SudokuBoard board);

#endif  //SUDOKU_H_
