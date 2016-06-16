#ifndef SOLVER_H
#define SOLVER_H
#include "sudoku.h"
#include <glib.h>
#include <assert.h>

bool solve_simple(sudoku_puzzle_t *puzzle);

GList *solve_diverge(sudoku_puzzle_t *puzzle);

#endif
