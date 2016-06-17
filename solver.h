#ifndef SOLVER_H
#define SOLVER_H
#include "sudoku.h"
#include "messages.h"
#include <glib.h>
#include <assert.h>

response_t solve_sudoku(request_t *request);

#endif
