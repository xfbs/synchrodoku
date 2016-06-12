#ifndef SUDOKU_H
#define SUDOKU_H
#include <stdbool.h>
#include <jansson.h>
#define SUDOKU_PACKED_SIZE 92

typedef struct sudoku_cell {
    bool numbers[9];
} sudoku_cell_t;

typedef struct sudoku_puzzle {
    sudoku_cell_t cells[9][9];
} sudoku_puzzle_t;

bool sudoku_cell_solved(const sudoku_cell_t *cell);

bool sudoku_cell_solvable(const sudoku_cell_t *cell);

int sudoku_cell_solution(const sudoku_cell_t *cell);

int sudoku_cell_candidates(const sudoku_cell_t *cell);

sudoku_cell_t sudoku_cell_empty();

sudoku_cell_t sudoku_cell_single(int num);

sudoku_cell_t sudoku_cell_new(int poss[]);

sudoku_cell_t sudoku_cell_intersect(const sudoku_cell_t *a, const sudoku_cell_t *b);

sudoku_puzzle_t sudoku_puzzle_empty();

sudoku_puzzle_t sudoku_puzzle_new(int numbers[9][9]);

sudoku_cell_t *sudoku_puzzle_cell(sudoku_puzzle_t *puzzle, int row, int col);

bool sudoku_puzzle_solved(const sudoku_puzzle_t *puzzle);

bool sudoku_puzzle_solvable(const sudoku_puzzle_t *puzzle);

bool sudoku_puzzle_valid(const sudoku_puzzle_t *puzzle);

sudoku_cell_t sudoku_puzzle_candidates_row(const sudoku_puzzle_t *puzzle, int row);

sudoku_cell_t sudoku_puzzle_candidates_col(const sudoku_puzzle_t *puzzle, int col);

json_t *sudoku_puzzle_to_json(const sudoku_puzzle_t *puzzle);

sudoku_puzzle_t *sudoku_puzzle_from_json(json_t *json);

void sudoku_puzzle_pack(unsigned char out[SUDOKU_PACKED_SIZE], const sudoku_puzzle_t *puzzle);

sudoku_puzzle_t sudoku_puzzle_unpack(const unsigned char packed[SUDOKU_PACKED_SIZE]);

#endif
