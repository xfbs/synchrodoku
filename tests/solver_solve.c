#include "helpers.h"

TEST(solve_works_with_empty_sudoku) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();
    assertEquals(solve_simple(&puzzle), false);
}

