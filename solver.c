#include "solver.h"

bool solve_simple(sudoku_puzzle_t *puzzle) {
    bool changed;

    do {
        changed = false;

        sudoku_cell_t rows[9];
        sudoku_cell_t cols[9];
        sudoku_cell_t squares[3][3];

        // setup the candidates for the rows
        // and columns
        for(int i = 0; i < 9; i++) {
            rows[i] = sudoku_puzzle_candidates_row(puzzle, i);
            cols[i] = sudoku_puzzle_candidates_col(puzzle, i);
        }

        // setup the candidates of the squares
        for(int row = 0; row < 3; row++) {
            for(int col = 0; col < 3; col++) {
                squares[row][col] = sudoku_puzzle_candidates_square(puzzle, 3*row, 3*col);
            }
        }

        for(int row = 0; row < 9; row++) {
            for(int col = 0; col < 9; col++) {
                sudoku_cell_t *cell = sudoku_puzzle_cell(puzzle, row, col);
                if(!sudoku_cell_solved(cell)) {
                    sudoku_cell_t intersect;
                    intersect = sudoku_cell_intersect(&rows[row], &cols[col]);
                    intersect = sudoku_cell_intersect(&intersect, &squares[row/3][col/3]);
                    intersect = sudoku_cell_intersect(&intersect, cell);

                    if(sudoku_cell_candidates(&intersect) < sudoku_cell_candidates(cell)) {
                        changed = true;
                        *cell = intersect;
                    }
                }
            }
        }
    } while(changed);

    return sudoku_puzzle_solved(puzzle);
}

GList *solve_diverge(sudoku_puzzle_t *puzzle) {
    if(!sudoku_puzzle_solvable(puzzle)) {
        return NULL;
    }

    GList *results = g_list_alloc();
    assert(results != NULL);

    // find the cell with the smallest amount
    // of candidates
    int div_row = 0;
    int div_col = 0;
    int div_candidates = 9;
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            sudoku_cell_t *cell = sudoku_puzzle_cell(puzzle, row, col);
            int cur_candidates = sudoku_cell_candidates(cell);
            if(cur_candidates < div_candidates) {
                div_row = row;
                div_col = col;
                div_candidates = cur_candidates;
            }
        }
    }

    // this is the cell we are diverging upon
    sudoku_cell_t *div_cell = sudoku_puzzle_cell(puzzle, div_row, div_col);

    for(int n = 0; n < 9; n++) {
        if(!div_cell->numbers[n]) continue;

        sudoku_puzzle_t *div = malloc(sizeof(sudoku_puzzle_t));
        assert(div != NULL);
        *div = *puzzle;

        sudoku_cell_t *new_cell = sudoku_puzzle_cell(puzzle, div_row, div_col);
        
        for(int i = 0; i < 9; i++) {
            if(i != n) {
                new_cell->numbers[i] = false;
            }
        }

        g_list_append(results, div);
    }

    return results;
}

