#include "sudoku.h"
#include <assert.h>

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

const sudoku_cell_t *sudoku_puzzle_cell_const(const sudoku_puzzle_t *puzzle, int row, int col) {
    return &puzzle->cells[row][col];
}

bool sudoku_puzzle_solved(const sudoku_puzzle_t *puzzle) {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(!sudoku_cell_solved(sudoku_puzzle_cell_const(puzzle, row, col))) {
                return false;
            }
        }
    }

    return true;
}

bool sudoku_puzzle_solvable(const sudoku_puzzle_t *puzzle) {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(!sudoku_cell_solvable(sudoku_puzzle_cell_const(puzzle, row, col))) {
                return false;
            }
        }
    }

    return true;
}

bool sudoku_puzzle_valid(const sudoku_puzzle_t *puzzle) {
    return true;
}

sudoku_cell_t sudoku_puzzle_candidates_row(const sudoku_puzzle_t *puzzle, int row) {
    sudoku_cell_t cell = sudoku_cell_any();

    // for any cell that has a solution, subtract that
    // from the list of possible solutions
    for(int i = 0; i < 9; i++) {
        int solution = sudoku_cell_solution(sudoku_puzzle_cell_const(puzzle, row, i));

        if(solution != 0) {
            cell.numbers[solution-1] = false;
        }
    }

    return cell;
}

sudoku_cell_t sudoku_puzzle_candidates_col(const sudoku_puzzle_t *puzzle, int col) {
    sudoku_cell_t cell = sudoku_cell_any();

    // for any cell that has a solution, subtract that
    // from the list of possible solutions
    for(int i = 0; i < 9; i++) {
        int solution = sudoku_cell_solution(sudoku_puzzle_cell_const(puzzle, i, col));

        if(solution != 0) {
            cell.numbers[solution-1] = false;
        }
    }

    return cell;
}

sudoku_cell_t sudoku_puzzle_candidates_square(const sudoku_puzzle_t *puzzle, int row, int col) {
    // adjust row and col to point to upper left cell
    // of the square
    row -= row % 3;
    col -= col % 3;
    sudoku_cell_t cell = sudoku_cell_any();

    for(int r = 0; r < 3; r++) {
        for(int c = 0; c < 3; c++) {
            int solution = sudoku_cell_solution(sudoku_puzzle_cell_const(puzzle, row+r, col+c));

            if(solution != 0) {
                cell.numbers[solution-1] = false;
            }
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

json_t *sudoku_puzzle_to_json(const sudoku_puzzle_t *puzzle) {
    json_t *json = json_array();

    for(int row = 0; row < 9; row++) {
        json_t *jrow = json_array();

        for(int col = 0; col < 9; col++) {
            int solution = sudoku_cell_solution(sudoku_puzzle_cell_const(puzzle, row, col));

            if(solution != 0) {
                // TODO: figure out if we should use this function
                // or array_append_new (incref?)
                json_array_append(jrow, json_integer(solution));
            } else {
                const sudoku_cell_t *cell = sudoku_puzzle_cell_const(puzzle, row, col);
                json_t *solutions = json_array();

                for(int n = 0; n < 9; n++) {
                    if(cell->numbers[n]) {
                        json_array_append(solutions, json_integer(n+1));
                    }
                }

                json_array_append(jrow, solutions);
            }
        }

        json_array_append(json, jrow);
    }

    return json;
}

int sudoku_puzzle_from_json_parse_row(sudoku_puzzle_t *puzzle, json_t *jrow, int row);
sudoku_cell_t sudoku_puzzle_from_json_parse_cell(json_t *jcell);

sudoku_puzzle_t sudoku_puzzle_from_json(json_t *json) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();

    if(!json_is_array(json)) {
        fprintf(stderr, "something bad happened!\n");
        return puzzle;
    }

    // iterare thru json array
    for(int row = 0; row < json_array_size(json); row++) {
        json_t *jrow = json_array_get(json, row);
        sudoku_puzzle_from_json_parse_row(&puzzle, jrow, row);
    }

    return puzzle;
}

int sudoku_puzzle_from_json_parse_row(sudoku_puzzle_t *puzzle, json_t *jrow, int row) {
    if(!json_is_array(jrow)) {
        fprintf(stderr, "something bad happened!\n");
        return -1;
    }

    for(int col = 0; col < json_array_size(jrow); col++) {
        json_t *jcell = json_array_get(jrow, col);
        sudoku_cell_t *cell = sudoku_puzzle_cell(puzzle, row, col);
        *cell = sudoku_puzzle_from_json_parse_cell(jcell);
    }

    return 0;
}

sudoku_cell_t sudoku_puzzle_from_json_parse_cell(json_t *jcell) {
    sudoku_cell_t cell = sudoku_cell_empty();

    if(json_is_integer(jcell)) {
        // extract integer
        int val = json_integer_value(jcell);

        // make sure integer is in range because
        // we are using it as an index
        if(val >= 1 && val <= 9) {
            cell.numbers[val-1] = true;
        } else {
            fprintf(stderr, "something bad happened!\n");
        }
    } else if(json_is_array(jcell)) {
        for(int n = 0; n < json_array_size(jcell); n++) {
            json_t *jcandidate = json_array_get(jcell, n);
            if(json_is_integer(jcandidate)) {
                // extract integer
                int val = json_integer_value(jcandidate);

                // make sure integer is in range because
                // we are using it as an index
                if(val >= 1 && val <= 9) {
                    cell.numbers[val-1] = true;
                } else {
                    fprintf(stderr, "something bad happened!\n");
                }
            } else {
                fprintf(stderr, "something bad happened!\n");
            }
        }
    } else {
        fprintf(stderr, "something bad happened!\n");
    }

    return cell;
}

void sudoku_puzzle_pack(unsigned char packed[92], const sudoku_puzzle_t *puzzle) {
    int pos = 0;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            const sudoku_cell_t *cell = sudoku_puzzle_cell_const(puzzle, r, c);

            packed[pos] = 0;

            for(int n = 0; n < 8; n++) {
                packed[pos] <<= 1;
                packed[pos] += (cell->numbers[n]) ? 1 : 0;
            }

            pos++;
        }
    }

    for(int r = 0; r < 9; r++) {
        packed[pos] = 0;

        for(int c = 0; c < 8; c++) {
            packed[pos] <<= 1;

            const sudoku_cell_t *cell = sudoku_puzzle_cell_const(puzzle, r, c);

            packed[pos] += cell->numbers[8] ? 1 : 0;
        }

        pos++;
    }

    packed[pos] = 0;

    for(int r = 0; r < 8; r++) {
        packed[pos] <<= 1;

        const sudoku_cell_t *cell = sudoku_puzzle_cell_const(puzzle, r, 8);

        packed[pos] += cell->numbers[8] ? 1 : 0;
    }

    pos++;
    packed[pos] = sudoku_puzzle_cell_const(puzzle, 8, 8)->numbers[8] ? 255 : 0;

    assert(pos == 91);
}

sudoku_puzzle_t sudoku_puzzle_unpack(const unsigned char packed[92]) {
    sudoku_puzzle_t puzzle;
    sudoku_cell_t *cell;
    int pos = 0;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            cell = sudoku_puzzle_cell(&puzzle, r, c);

            for(int n = 0; n < 8; n++) {
                cell->numbers[n] = (packed[pos] >> (7-n)) & 1;
            }

            pos++;
        }
    }

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 8; c++) {
            cell = sudoku_puzzle_cell(&puzzle, r, c);

            cell->numbers[8] = (packed[pos] >> (7-c)) & 1;
        }

        pos++;
    }

    for(int r = 0; r < 8; r++) {
        cell = sudoku_puzzle_cell(&puzzle, r, 8);
        cell->numbers[8] = (packed[pos] >> (7-r)) & 1;
    }

    pos++;
    cell = sudoku_puzzle_cell(&puzzle, 8, 8);
    cell->numbers[8] = packed[pos] >> 7;

    return puzzle;
}

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

sudoku_cell_t sudoku_cell_single(int num) {
    sudoku_cell_t cell = sudoku_cell_empty();

    if(num >= 1 && num <= 9) {
        cell.numbers[num-1] = true;
    }

    return cell;
}

sudoku_cell_t sudoku_cell_new(int poss[]) {
    sudoku_cell_t cell = sudoku_cell_empty();

    for(int i = 0; poss[i] != 0; i++) {
        if(poss[i] >= 1 && poss[i] <= 9) {
            cell.numbers[poss[i]-1] = true;
        }
    }

    return cell;
}

sudoku_cell_t sudoku_cell_any() {
    sudoku_cell_t cell;

    for(int n = 0; n < 9; n++) {
        cell.numbers[n] = true;
    }

    return cell;
}

sudoku_puzzle_t sudoku_puzzle_new(int numbers[9][9]) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            sudoku_cell_t *cell = sudoku_puzzle_cell(&puzzle, r, c);

            if(numbers[r][c] != 0) {
                *cell = sudoku_cell_single(numbers[r][c]);
            } else {
                *cell = sudoku_cell_any();
            }
        }
    }

    return puzzle;
}

bool sudoku_solve_simple(sudoku_puzzle_t *puzzle) {
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

GList *sudoku_solve_diverge(sudoku_puzzle_t *puzzle) {
    if(!sudoku_puzzle_solvable(puzzle)) {
        return NULL;
    }

    // we are looking for the cell with the smallest
    // amount of candidates, this is the starting
    // point
    int div_row = 0;
    int div_col = 0;
    int div_candidates = sudoku_cell_candidates(sudoku_puzzle_cell(puzzle, 0, 0));;
    
    // go through all cells to find it
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            sudoku_cell_t *cell = sudoku_puzzle_cell(puzzle, row, col);
            int cur_candidates = sudoku_cell_candidates(cell);

            // if the number of candidates is 1, the cell
            // is solved and we are not interested in it.
            if(cur_candidates > 1 && cur_candidates < div_candidates) {
                div_row = row;
                div_col = col;
                div_candidates = cur_candidates;
            }
        }
    }

    GList *results = NULL;
    sudoku_cell_t *div_cell = sudoku_puzzle_cell(puzzle, div_row, div_col);

    for(int n = 0; n < 9; n++) {
        if(!div_cell->numbers[n]) continue;

        // allocate new sudoku
        sudoku_puzzle_t *div = malloc(sizeof(sudoku_puzzle_t));
        assert(div != NULL);

        // copy puzzle into new sudoku
        *div = *puzzle;

        // but change the cell we are divering
        // upon to whatever we want to set it
        sudoku_cell_t *new_cell = sudoku_puzzle_cell(div, div_row, div_col);
        for(int i = 0; i < 9; i++) {
            if(i != n) {
                new_cell->numbers[i] = false;
            }
        }

        // add diverged sudoku to list of
        // results
        results = g_list_append(results, div);
    }

    return results;
}

