#include "helpers.h"

TEST(solve_works_with_empty_sudoku) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();
    sudoku_puzzle_t solved = puzzle;
    assertEquals(solve_simple(&solved), false);
    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &solved), true);
}

TEST(solve_works_with_solved_sudoku) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_new((int[9][9]){
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 1, 2, 3},
            {7, 8, 9, 1, 2, 3, 4, 5, 6},
            {2, 3, 4, 5, 6, 7, 8, 9, 1},
            {5, 6, 7, 8, 9, 1, 2, 3, 4},
            {8, 9, 1, 2, 3, 4, 5, 6, 7},
            {3, 4, 5, 6, 7, 8, 9, 1, 2},
            {6, 7, 8, 9, 1, 2, 3, 4, 5},
            {9, 1, 2, 3, 4, 5, 6, 7, 8}});

    sudoku_puzzle_t solved = puzzle;

    assertEquals(sudoku_puzzle_solved(&puzzle), true);
    assertEquals(sudoku_puzzle_valid(&puzzle), true);
    assertEquals(solve_simple(&solved), true);
    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &solved), true);
}
