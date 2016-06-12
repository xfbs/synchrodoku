#include "helpers.h"

TEST(puzzle_new_works_with_full_sudoku) {
    int numbers[9][9] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 1, 2, 3, 4, 5, 6},
        {2, 3, 4, 5, 6, 7, 8, 9, 1},
        {5, 6, 7, 8, 9, 1, 2, 3, 4},
        {8, 9, 1, 2, 3, 4, 5, 6, 7},
        {3, 4, 5, 6, 7, 8, 9, 1, 2},
        {6, 7, 8, 9, 1, 2, 3, 4, 5},
        {9, 1, 2, 3, 4, 5, 6, 7, 8}
    };

    sudoku_puzzle_t puzzle = sudoku_puzzle_new(numbers);

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t *cell = sudoku_puzzle_cell(&puzzle, r, c);
            assertEquals(sudoku_cell_solution(cell), numbers[r][c]);
        }
    }
}

TEST(puzzle_new_works_with_empty_sudoku) {
    int numbers[9][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    sudoku_puzzle_t puzzle = sudoku_puzzle_new(numbers);
    sudoku_puzzle_t empty = sudoku_puzzle_empty();
    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &empty), true);
}

TEST(puzzle_cell_works) {
    // TODO
}

TEST(puzzle_solved_works) {
    // TODO
}

TEST(puzzle_solvable_works) {
    // TODO
}

TEST(puzzle_valid_works) {
    // TODO
}

TEST(puzzle_candidates_row_works) {
    // TODO
}

TEST(puzzle_candidates_col_worls) {
    // TODO
}


