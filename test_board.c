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

static char *all_tests() {
  mu_run_test(test_clear_hints);
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
