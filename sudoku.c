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

bool sudoku_puzzle_valid(const sudoku_puzzle_t *puzzle) {
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

json_t *sudoku_puzzle_to_json(const sudoku_puzzle_t *puzzle) {
    json_t *json = json_array();

    for(int row = 0; row < 9; row++) {
        json_t *jrow = json_array();

        for(int col = 0; col < 9; col++) {
            int solution = sudoku_cell_solution(_sudoku_puzzle_cell(puzzle, row, col));

            if(solution != 0) {
                // TODO: figure out if we should use this function
                // or array_append_new (incref?)
                json_array_append(jrow, json_integer(solution));
            } else {
                const sudoku_cell_t *cell = _sudoku_puzzle_cell(puzzle, row, col);
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

sudoku_puzzle_t *sudoku_puzzle_from_json(json_t *json) {
    if(!json_is_array(json)) {
        return NULL;
    }

    sudoku_puzzle_t *puzzle = malloc(sizeof(sudoku_puzzle_t));;
    assert(puzzle != NULL);

    *puzzle = sudoku_puzzle_empty();

    // iterare thru json array
    // TODO fix jansson calls (memory safety!)
    int col, row, n;
    json_t *jcol, *jcell, *jcandidate;
    json_array_foreach(json, col, jcol) {
        if(!json_is_array(jcol)) {
            goto error;
        }

        json_array_foreach(jcol, row, jcell) {
            sudoku_cell_t *cell = sudoku_puzzle_cell(puzzle, row, col);
            if(json_is_integer(jcell)) {
                // extract integer
                int val = json_integer_value(jcell);

                // make sure integer is in range because
                // we are using it as an index
                if(val < 1 || val > 9) goto error;

                cell->numbers[val-1] = true;
            } else if(json_is_array(jcell)) {
                json_array_foreach(jcell, n, jcandidate) {
                    if(json_is_integer(jcandidate)) {
                        // extract integer
                        int val = json_integer_value(jcandidate);

                        // make sure integer is in range because
                        // we are using it as an index
                        if(val < 1 || val > 9) goto error;

                        cell->numbers[val-1] = true;
                    } else {
                        goto error;
                    }
                }
            } else {
                goto error;
            }
        }
    }

    return puzzle;

error:
    free(puzzle);
    return NULL;
}

void sudoku_puzzle_pack(unsigned char packed[92], const sudoku_puzzle_t *puzzle) {
    int pos = 0;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            const sudoku_cell_t *cell = _sudoku_puzzle_cell(puzzle, r, c);

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

            const sudoku_cell_t *cell = _sudoku_puzzle_cell(puzzle, r, c);

            packed[pos] += cell->numbers[8] ? 1 : 0;
        }

        pos++;
    }

    packed[pos] = 0;

    for(int r = 0; r < 8; r++) {
        packed[pos] <<= 1;

        const sudoku_cell_t *cell = _sudoku_puzzle_cell(puzzle, r, 8);

        packed[pos] += cell->numbers[8] ? 1 : 0;
    }

    pos++;
    packed[pos] = _sudoku_puzzle_cell(puzzle, 8, 8)->numbers[8] ? 255 : 0;

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

            if(cell != 0) {
                *cell = sudoku_cell_single(numbers[r][c]);
            } else {
                *cell = sudoku_cell_any();
            }
        }
    }

    return puzzle;
}
