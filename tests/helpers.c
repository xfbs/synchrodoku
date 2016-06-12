#include "helpers.h"

bool sudoku_puzzle_equals_strict(sudoku_puzzle_t *a, sudoku_puzzle_t *b) {
    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t *a_cell = sudoku_puzzle_cell(a, r, c);
            sudoku_cell_t *b_cell = sudoku_puzzle_cell(b, r, c);

            for(int n = 0; n < 9; n++) {
                if(a_cell->numbers[n] != b_cell->numbers[n]) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool sudoku_puzzle_equals(sudoku_puzzle_t *a, sudoku_puzzle_t *b) {
    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t *a_cell = sudoku_puzzle_cell(a, r, c);
            sudoku_cell_t *b_cell = sudoku_puzzle_cell(b, r, c);

            for(int n = 0; n < 9; n++) {
                if(sudoku_cell_solution(a_cell) != sudoku_cell_solution(b_cell)) {
                    return false;
                }
            }
        }
    }

    return true;
}

