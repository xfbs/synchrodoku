#include "helpers.h"

sudoku_puzzle_t sudoku_puzzle_empty() {
    sudoku_puzzle_t puzzle;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t *cell = sudoku_puzzle_cell(&puzzle, r, c);

            *cell = sudoku_cell_empty();
        }
    }

    return puzzle;
}

sudoku_cell_t sudoku_cell_empty() {
    sudoku_cell_t cell;

    for(int n = 0; n < 9; n++) {
        cell.numbers[n] = false;
    }

    return cell;
}
