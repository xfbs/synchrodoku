#include "sudoku.h"

int sudoku_cell_candidates(const sudoku_cell_t *cell) {
    int candidates = 0;

    for(int i = 0; i < 9; i++) {
        if(cell->numbers[i]) {
            candidates++;
        }
    }

    return candidates;
}


bool sudoku_cell_solved(const sudoku_cell_t *cell) {
    return (sudoku_cell_candidates(cell) == 1);
}

bool sudoku_cell_solvable(const sudoku_cell_t *cell) {
    return (sudoku_cell_candidates(cell) >= 1);
}

int sudoku_cell_solution(const sudoku_cell_t *cell) {
    int solution = 0;

    for(int i = 0; i < 9; i++) {
        if(cell->numbers[i]) {
            if(!solution) {
                solution = i+1;
            } else {
                return 0;
            }
        }
    }

    return solution;
}

sudoku_cell_t *sudoku_puzzle_cell(sudoku_puzzle_t *puzzle, int row, int col) {
    return &puzzle->cells[row][col];
}

const sudoku_cell_t *_sudoku_puzzle_cell(const sudoku_puzzle_t *puzzle, int row, int col) {
    return &puzzle->cells[row][col];
}

bool sudoku_puzzle_solved(const sudoku_puzzle_t *puzzle) {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(!sudoku_cell_solved(_sudoku_puzzle_cell(puzzle, row, col))) {
                return false;
            }
        }
    }

    return true;
}

bool sudoku_puzzle_solvable(const sudoku_puzzle_t *puzzle) {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(!sudoku_cell_solvable(_sudoku_puzzle_cell(puzzle, row, col))) {
                return false;
            }
        }
    }

    return true;
}

sudoku_cell_t sudoku_puzzle_candidates_row(const sudoku_puzzle_t *puzzle, int row) {
    const sudoku_puzzle_t *p = (const sudoku_puzzle_t *) puzzle;
    sudoku_cell_t cell;

    // initialise all numbers to possible
    for(int i = 0; i < 9; i++) cell.numbers[i] = true;

    // for any cell that has a solution, subtract that
    // from the list of possible solutions
    for(int i = 0; i < 9; i++) {
        int solution = sudoku_cell_solution(_sudoku_puzzle_cell(p, row, i));

        if(solution != 0) {
            cell.numbers[solution-1] = false;
        }
    }

    return cell;
}

sudoku_cell_t sudoku_puzzle_candidates_col(const sudoku_puzzle_t *puzzle, int col) {
    const sudoku_puzzle_t *p = (const sudoku_puzzle_t *) puzzle;
    sudoku_cell_t cell;

    // initialise all numbers to possible
    for(int i = 0; i < 9; i++) cell.numbers[i] = true;

    // for any cell that has a solution, subtract that
    // from the list of possible solutions
    for(int i = 0; i < 9; i++) {
        int solution = sudoku_cell_solution(_sudoku_puzzle_cell(p, i, col));

        if(solution != 0) {
            cell.numbers[solution-1] = false;
        }
    }

    return cell;
}

sudoku_cell_t sudoku_cell_intersect(const sudoku_cell_t *a, const sudoku_cell_t *b) {
    sudoku_cell_t cell = *a;

    for(int i = 0; i < 9; i++) {
        if(!b->numbers[i]) {
            cell.numbers[i] = false;
        }
    }

    return cell;
}
