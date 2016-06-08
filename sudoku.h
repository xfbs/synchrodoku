#ifndef SUDOKU_H
#define SUDOKU_H
#include <stdbool.h>

typedef struct sudoku_cell {
    bool numbers[9];
} sudoku_cell_t;

typedef struct sudoku_puzzle {
    sudoku_cell_t cells[9][9];
} sudoku_puzzle_t;

bool sudoku_cell_solved(const sudoku_cell_t *cell);

bool sudoku_cell_solvable(const sudoku_cell_t *cell);

int sudoku_cell_solution(const sudoku_cell_t *cell);

sudoku_cell_t *sudoku_puzzle_cell(sudoku_puzzle_t *puzzle, int row, int col);

bool sudoku_puzzle_solved(const sudoku_puzzle_t *puzzle);

bool sudoku_puzzle_solvable(const sudoku_puzzle_t *puzzle);

sudoku_cell_t sudoku_puzzle_candidates_row(const sudoku_puzzle_t *puzzle, int row);

sudoku_cell_t sudoku_puzzle_candidates_col(const sudoku_puzzle_t *puzzle, int col);

sudoku_cell_t sudoku_cell_intersect(const sudoku_cell_t *a, const sudoku_cell_t *b);

#endif
