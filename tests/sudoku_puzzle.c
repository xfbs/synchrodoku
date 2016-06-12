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

/*
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
*/

TEST(puzzle_empty_works) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t empty = sudoku_cell_empty();
            sudoku_cell_t *cell = sudoku_puzzle_cell(&puzzle, r, c);

            assertEquals(sudoku_cell_equals_strict(&empty, cell), true);
        }
    }
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
    sudoku_puzzle_t puzzle = sudoku_puzzle_new((int[9][9]){
        {1, 0, 3, 4, 5, 6, 7, 8, 9},
        {4, 0, 0, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 0, 0, 0, 4, 5, 6},
        {0, 0, 0, 0, 0, 0, 0, 9, 0},
        {0, 6, 7, 8, 9, 1, 2, 3, 4},
        {8, 9, 1, 2, 3, 4, 5, 6, 0},
        {0, 4, 5, 6, 7, 8, 0, 1, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {9, 1, 2, 3, 4, 5, 6, 7, 8}});

    sudoku_cell_t candidates, expected;

    candidates = sudoku_puzzle_candidates_row(&puzzle, 0);
    expected = sudoku_cell_new((int[]){2, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 1);
    expected = sudoku_cell_new((int[]){5, 6, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 2);
    expected = sudoku_cell_new((int[]){1, 2, 3, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 3);
    expected = sudoku_cell_new((int[]){1, 2, 3, 4, 5, 6, 7, 8, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 4);
    expected = sudoku_cell_new((int[]){5, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 5);
    expected = sudoku_cell_new((int[]){7, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 6);
    expected = sudoku_cell_new((int[]){3, 9, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 7);
    expected = sudoku_cell_new((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);

    candidates = sudoku_puzzle_candidates_row(&puzzle, 8);
    expected = sudoku_cell_new((int[]){0});
    assertEquals(sudoku_cell_equals_strict(&candidates, &expected), true);
}

TEST(puzzle_candidates_col_worls) {
    // TODO
}


