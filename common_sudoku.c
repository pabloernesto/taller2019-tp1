#include "sudoku.h"

#include <string.h>

const SudokuBoard sector_map = {
  { 0, 0, 0, 1, 1, 1, 2, 2, 2, },
  { 0, 0, 0, 1, 1, 1, 2, 2, 2, },
  { 0, 0, 0, 1, 1, 1, 2, 2, 2, },
  { 3, 3, 3, 4, 4, 4, 5, 5, 5, },
  { 3, 3, 3, 4, 4, 4, 5, 5, 5, },
  { 3, 3, 3, 4, 4, 4, 5, 5, 5, },
  { 6, 6, 6, 7, 7, 7, 8, 8, 8, },
  { 6, 6, 6, 7, 7, 7, 8, 8, 8, },
  { 6, 6, 6, 7, 7, 7, 8, 8, 8 }
};

int Sudoku_IsSolved(const struct Sudoku *game) {
  if (!game) return -1;

  char rows[9][9] = {{ 0 }};
  char columns[9][9] = {{ 0 }};
  char sectors[9][9] = {{ 0 }};

  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      char cell = game->board[i][j];
      // convert to index
      cell -= 49;
      if (cell < 0 || cell > 9)
        return -1;

      if (rows[i][(int) cell]) return 0;
      if (columns[j][(int) cell]) return 0;
      if (sectors[(int) sector_map[i][j]][(int) cell]) return 0;

      rows[i][(int) cell] = 1;
      columns[j][(int) cell] = 1;
      sectors[(int) sector_map[i][j]][(int) cell] = 1;
    }
  return 1;
}

void SudokuBoard_Clear(SudokuBoard board) {
  memset(board, '0', sizeof(SudokuBoard));
}
