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

int Sudoku_Put(struct Sudoku *game, int row, int col, int val) {
  if (game->hints[row][col] != '0') return -1;

  game->board[row][col] = val;
  return 0;
}

const char *Sudoku_Pretty(const struct Sudoku *game) {
  static char pretty[] =
    "U===========U===========U===========U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U---+---+---U---+---+---U---+---+---U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U---+---+---U---+---+---U---+---+---U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U===========U===========U===========U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U---+---+---U---+---+---U---+---+---U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U---+---+---U---+---+---U---+---+---U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U===========U===========U===========U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U---+---+---U---+---+---U---+---+---U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U---+---+---U---+---+---U---+---+---U\n"
    "U X | X | X U X | X | X U X | X | X U\n"
    "U===========U===========U===========U\n";
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      const int width = 38;
      const int array_index = 2 + 4*j + width * (1 + 2*i);

      if (game->board[i][j] != '0')
        pretty[array_index] = game->board[i][j];
      else if (game->hints[i][j] != '0')
        pretty[array_index] = game->hints[i][j];
      else
        pretty[array_index] = ' ';
    }
  }
  return pretty;
}
