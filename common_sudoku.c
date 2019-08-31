#include "sudoku.h"

#include <string.h>

int Sudoku_IsSolved(const struct Sudoku *game) {
  if (!game) return -1;
  return 1;
}

void SudokuBoard_Clear(SudokuBoard board) {
  memset(board, '0', sizeof(SudokuBoard));
}
