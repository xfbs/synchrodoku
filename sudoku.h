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

// returns true if the cell has a single solution
bool sudoku_cell_solved(const sudoku_cell_t *cell);

// returns true if the cell has a single solution
// or more than one candidates
bool sudoku_cell_solvable(const sudoku_cell_t *cell);

// returns the solution of the cell, or 0 if it
// doesn't have one
int sudoku_cell_solution(const sudoku_cell_t *cell);

// returns how many candidates the cell has
int sudoku_cell_candidates(const sudoku_cell_t *cell);

// returns a cell with no candidates
sudoku_cell_t sudoku_cell_empty();

// returns a cell with a single solution, num
sudoku_cell_t sudoku_cell_single(int num);

// returns a new cell with the specified solutions,
// poss is an array of the solution with 0 denoting
// the end of the array
sudoku_cell_t sudoku_cell_new(int poss[]);

// returns a cell with all candidates set
sudoku_cell_t sudoku_cell_any();

// return the intersection of two cells, that is
// a cell that has all candidates set that are
// also candidates in both `a` and `b`
sudoku_cell_t sudoku_cell_intersect(const sudoku_cell_t *a, const sudoku_cell_t *b);

// return a sudoku consisting of only cells
// that don't have a solution
sudoku_puzzle_t sudoku_puzzle_empty();

// construct a new sudoku using the given array as the
// cells. 1-9 means that the cell has the specified
// number as only solution, 0 means all candidates are
// set, and anything else means that no candidate is
// set
sudoku_puzzle_t sudoku_puzzle_new(int numbers[9][9]);

// returns a pointer to the cell at the given row and
// column of the sudoku
sudoku_cell_t *sudoku_puzzle_cell(sudoku_puzzle_t *puzzle, int row, int col);

// returns a const pointer to the cell at the given 
// row and column of the sudoku
const sudoku_cell_t *sudoku_puzzle_cell_const(const sudoku_puzzle_t *puzzle, int row, int col);

// returns true if all cells in the sudoku have a
// solution
bool sudoku_puzzle_solved(const sudoku_puzzle_t *puzzle);

// returns true if there are no cells in the sudoku
// which have no candidate
bool sudoku_puzzle_solvable(const sudoku_puzzle_t *puzzle);

// returns true if the sudoku is valid according to
// the rules of sudokus
bool sudoku_puzzle_valid(const sudoku_puzzle_t *puzzle);

// returns a cell representing the candidates of
// empty cells in the given row
sudoku_cell_t sudoku_puzzle_candidates_row(const sudoku_puzzle_t *puzzle, int row);

// returns a cell representing the candidates of
// empty cells in the given column
sudoku_cell_t sudoku_puzzle_candidates_col(const sudoku_puzzle_t *puzzle, int col);

// returns a cell representing the candidates of
// empty cells in the given square
sudoku_cell_t sudoku_puzzle_candidates_square(const sudoku_puzzle_t *puzzle, int row, int col);

// convert a sudoku to json format using the jansson library
json_t *sudoku_puzzle_to_json(const sudoku_puzzle_t *puzzle);

// read a sudoku puzzle from json using the jansson library
sudoku_puzzle_t sudoku_puzzle_from_json(json_t *json);

// pack a sudoku into a binary representation
void sudoku_puzzle_pack(unsigned char out[SUDOKU_PACKED_SIZE], const sudoku_puzzle_t *puzzle);

// read a sudoku from a binary representation
sudoku_puzzle_t sudoku_puzzle_unpack(const unsigned char packed[SUDOKU_PACKED_SIZE]);

#endif
