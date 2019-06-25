// vim: noet:ts=4:sw=4
#ifndef MANAGER_H
#define MANAGER_H
#include <zmq.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include "sudoku.h"
#include "solver.h"

typedef struct {
  int thread_count;
} manager_options_t;

void manager_start(manager_options_t options);

sudoku_puzzle_t manager_solve_sudoku(sudoku_puzzle_t *puzzle);

void manager_stop(void);
void *manager_loop(void *data);

#endif
