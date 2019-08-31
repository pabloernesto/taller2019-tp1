#include <stdio.h>
#include "minunit.h"

#include "sudoku.h"

int tests_run = 0;

static char *test_clear_hints() {
  struct Sudoku game;
  SudokuBoard_Clear(game.hints);

  char x = game.hints[8][8];

  mu_assert("test_clear_hints failed: expected 0 in position [8,8]", x == '0');
  return 0;
}

static char *test_solved_board() {
  struct Sudoku game = { {
    { '7', '3', '5', '6', '1', '4', '8', '9', '2', },
    { '8', '4', '2', '9', '7', '3', '5', '6', '1', },
    { '9', '6', '1', '2', '8', '5', '3', '7', '4', },
    { '2', '8', '6', '3', '4', '9', '1', '5', '7', },
    { '4', '1', '3', '8', '5', '7', '9', '2', '6', },
    { '5', '7', '9', '1', '2', '6', '4', '3', '8', },
    { '1', '5', '7', '4', '9', '2', '6', '8', '3', },
    { '6', '9', '4', '7', '3', '8', '2', '1', '5', },
    { '3', '2', '8', '5', '6', '1', '7', '4', '9' }
  }, {{ 0 }} };
  SudokuBoard_Clear(game.hints);

  int r = Sudoku_IsSolved(&game);

  mu_assert("test_solved_board failed: expected 1", r == 1);
  return 0;
}

static char *all_tests() {
  mu_run_test(test_clear_hints);
  mu_run_test(test_solved_board);
  return 0;
}

int main(int argc, char **argv) {
  char *result = all_tests();

  if (result) printf("%s\n", result);
  else
    printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
