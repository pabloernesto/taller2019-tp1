#include "sudoku.h"

#include <string.h>

void SudokuBoard_Clear(SudokuBoard board) {
  memset(board, '0', sizeof(SudokuBoard));
}
