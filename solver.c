#include "solver.h"

bool solve_simple(sudoku_puzzle_t *puzzle) {
    bool changed;

    printf("solving sudoku: \n");
    json_dumpf(sudoku_puzzle_to_json(puzzle), stdout, 0);
    printf("\n");

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
                    printf("solving cell [%i][%i]\n", row, col);
                    printf("cell candidates are: %c%c%c%c%c%c%c%c%c\n",
                            cell->numbers[0] ? '1' : ' ',
                            cell->numbers[1] ? '2' : ' ',
                            cell->numbers[2] ? '3' : ' ',
                            cell->numbers[3] ? '4' : ' ',
                            cell->numbers[4] ? '5' : ' ',
                            cell->numbers[5] ? '6' : ' ',
                            cell->numbers[6] ? '7' : ' ',
                            cell->numbers[7] ? '8' : ' ',
                            cell->numbers[8] ? '9' : ' ');
                    printf("row candidates are: %c%c%c%c%c%c%c%c%c\n",
                            rows[row].numbers[0] ? '1' : ' ',
                            rows[row].numbers[1] ? '2' : ' ',
                            rows[row].numbers[2] ? '3' : ' ',
                            rows[row].numbers[3] ? '4' : ' ',
                            rows[row].numbers[4] ? '5' : ' ',
                            rows[row].numbers[5] ? '6' : ' ',
                            rows[row].numbers[6] ? '7' : ' ',
                            rows[row].numbers[7] ? '8' : ' ',
                            rows[row].numbers[8] ? '9' : ' ');
                    printf("col candidates are: %c%c%c%c%c%c%c%c%c\n",
                            cols[col].numbers[0] ? '1' : ' ',
                            cols[col].numbers[1] ? '2' : ' ',
                            cols[col].numbers[2] ? '3' : ' ',
                            cols[col].numbers[3] ? '4' : ' ',
                            cols[col].numbers[4] ? '5' : ' ',
                            cols[col].numbers[5] ? '6' : ' ',
                            cols[col].numbers[6] ? '7' : ' ',
                            cols[col].numbers[7] ? '8' : ' ',
                            cols[col].numbers[8] ? '9' : ' ');
                    printf("square candidates are: %c%c%c%c%c%c%c%c%c\n",
                            squares[row/3][col/3].numbers[0] ? '1' : ' ',
                            squares[row/3][col/3].numbers[1] ? '2' : ' ',
                            squares[row/3][col/3].numbers[2] ? '3' : ' ',
                            squares[row/3][col/3].numbers[3] ? '4' : ' ',
                            squares[row/3][col/3].numbers[4] ? '5' : ' ',
                            squares[row/3][col/3].numbers[5] ? '6' : ' ',
                            squares[row/3][col/3].numbers[6] ? '7' : ' ',
                            squares[row/3][col/3].numbers[7] ? '8' : ' ',
                            squares[row/3][col/3].numbers[8] ? '9' : ' ');
                    sudoku_cell_t intersect;
                    intersect = sudoku_cell_intersect(&rows[row], &cols[col]);
                    intersect = sudoku_cell_intersect(&intersect, &squares[row/3][col/3]);
                    intersect = sudoku_cell_intersect(&intersect, cell);
                    printf("chosen candidates are: %c%c%c%c%c%c%c%c%c\n",
                            intersect.numbers[0] ? '1' : ' ',
                            intersect.numbers[1] ? '2' : ' ',
                            intersect.numbers[2] ? '3' : ' ',
                            intersect.numbers[3] ? '4' : ' ',
                            intersect.numbers[4] ? '5' : ' ',
                            intersect.numbers[5] ? '6' : ' ',
                            intersect.numbers[6] ? '7' : ' ',
                            intersect.numbers[7] ? '8' : ' ',
                            intersect.numbers[8] ? '9' : ' ');

                    if(sudoku_cell_candidates(&intersect) < sudoku_cell_candidates(cell)) {
                        printf("intersection taken!\n");
                        changed = true;
                        *cell = intersect;
                    }
                }
            }
        }
    } while(changed);

    printf("done solving!\n");
    json_dumpf(sudoku_puzzle_to_json(puzzle), stdout, 0);
    printf("\n");

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

