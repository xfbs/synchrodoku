#include "cu/cu.h"
#include "solver.h"
#include "sudoku.h"
#include "messages.h"

bool sudoku_puzzle_equals_strict(const sudoku_puzzle_t *a, const sudoku_puzzle_t *b);
bool sudoku_puzzle_equals(const sudoku_puzzle_t *a, const sudoku_puzzle_t *b);
bool sudoku_cell_equals_strict(const sudoku_cell_t *a, const sudoku_cell_t *b);
bool sudoku_cell_equals(const sudoku_cell_t *a, const sudoku_cell_t *b);
