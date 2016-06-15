#include "helpers.h"

bool sudoku_puzzle_equals_strict(const sudoku_puzzle_t *a, const sudoku_puzzle_t *b) {
    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t *a_cell = sudoku_puzzle_cell(a, r, c);
            sudoku_cell_t *b_cell = sudoku_puzzle_cell(b, r, c);

            if(!sudoku_cell_equals_strict(a_cell, b_cell)) {
                return false;
            }
        }
    }

    return true;
}

bool sudoku_puzzle_equals(const sudoku_puzzle_t *a, const sudoku_puzzle_t *b) {
    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            const sudoku_cell_t *a_cell = sudoku_puzzle_cell_const(a, r, c);
            const sudoku_cell_t *b_cell = sudoku_puzzle_cell_const(b, r, c);

            if(!sudoku_cell_equals(a_cell, b_cell)) {
                return false;
            }
        }
    }

    return true;
}

bool sudoku_cell_equals(const sudoku_cell_t *a, const sudoku_cell_t *b) {
    return(sudoku_cell_solution(a) == sudoku_cell_solution(b));
}

bool sudoku_cell_equals_strict(const sudoku_cell_t *a, const sudoku_cell_t *b) {
    for(int n = 0; n < 9; n++) {
        if(a->numbers[n] != b->numbers[n]) {
            return false;
        }
    }

    return true;
}
