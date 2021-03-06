#include "helpers.h"

TEST(diverge_returns_solved_sudoku) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_new((int[9][9]){
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 1, 2, 3, 4, 5, 6},
        {2, 3, 4, 5, 6, 7, 8, 9, 1},
        {5, 6, 7, 8, 9, 1, 2, 3, 4},
        {8, 9, 1, 2, 3, 4, 5, 6, 7},
        {3, 4, 5, 6, 7, 8, 9, 1, 2},
        {6, 7, 8, 9, 1, 2, 3, 4, 5},
        {9, 1, 2, 3, 4, 5, 6, 7, 8}});

    GList *solved = sudoku_solve_diverge(&puzzle);
    assertEquals(g_list_length(solved), 1);
    sudoku_puzzle_t first = sudoku_puzzle_unpack(g_list_first(solved)->data);
    assertEquals(sudoku_puzzle_equals(&first, &puzzle), true);
    g_list_free_full(solved, (void (*)(void*))g_bytes_unref);
}

TEST(diverge_returns_correct_amount) {
    sudoku_puzzle_t puzzles[] = {
        sudoku_puzzle_new((int[9][9]){
            {0, 2, 3, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 0, 2, 3},
            {7, 8, 9, 0, 2, 3, 4, 5, 6},
            {2, 3, 4, 5, 6, 7, 8, 9, 0},
            {5, 6, 7, 8, 9, 0, 2, 3, 4},
            {8, 9, 0, 2, 3, 4, 5, 6, 7},
            {3, 4, 5, 6, 7, 8, 9, 0, 2},
            {6, 7, 8, 9, 0, 2, 3, 4, 5},
            {9, 0, 2, 3, 4, 5, 6, 7, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 3, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 0, 0, 3},
            {7, 8, 9, 0, 0, 3, 4, 5, 6},
            {0, 3, 4, 5, 6, 7, 8, 9, 0},
            {5, 6, 7, 8, 9, 0, 0, 3, 4},
            {8, 9, 0, 0, 3, 4, 5, 6, 7},
            {3, 4, 5, 6, 7, 8, 9, 0, 0},
            {6, 7, 8, 9, 0, 0, 3, 4, 5},
            {9, 0, 0, 3, 4, 5, 6, 7, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 0, 0, 0},
            {7, 8, 9, 0, 0, 0, 4, 5, 6},
            {0, 0, 4, 5, 6, 7, 8, 9, 0},
            {5, 6, 7, 8, 9, 0, 0, 0, 4},
            {8, 9, 0, 0, 0, 4, 5, 6, 7},
            {0, 4, 5, 6, 7, 8, 9, 0, 0},
            {6, 7, 8, 9, 0, 0, 0, 4, 5},
            {9, 0, 0, 0, 4, 5, 6, 7, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 0, 5, 6, 7, 8, 9},
            {0, 5, 6, 7, 8, 9, 0, 0, 0},
            {7, 8, 9, 0, 0, 0, 0, 5, 6},
            {0, 0, 0, 5, 6, 7, 8, 9, 0},
            {5, 6, 7, 8, 9, 0, 0, 0, 0},
            {8, 9, 0, 0, 0, 0, 5, 6, 7},
            {0, 0, 5, 6, 7, 8, 9, 0, 0},
            {6, 7, 8, 9, 0, 0, 0, 0, 5},
            {9, 0, 0, 0, 0, 5, 6, 7, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 0, 0, 6, 7, 8, 9},
            {0, 0, 6, 7, 8, 9, 0, 0, 0},
            {7, 8, 9, 0, 0, 0, 0, 0, 6},
            {0, 0, 0, 0, 6, 7, 8, 9, 0},
            {0, 6, 7, 8, 9, 0, 0, 0, 0},
            {8, 9, 0, 0, 0, 0, 0, 6, 7},
            {0, 0, 0, 6, 7, 8, 9, 0, 0},
            {6, 7, 8, 9, 0, 0, 0, 0, 0},
            {9, 0, 0, 0, 0, 0, 6, 7, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 0, 0, 0, 7, 8, 9},
            {0, 0, 0, 7, 8, 9, 0, 0, 0},
            {7, 8, 9, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 7, 8, 9, 0},
            {0, 0, 7, 8, 9, 0, 0, 0, 0},
            {8, 9, 0, 0, 0, 0, 0, 0, 7},
            {0, 0, 0, 0, 7, 8, 9, 0, 0},
            {0, 7, 8, 9, 0, 0, 0, 0, 0},
            {9, 0, 0, 0, 0, 0, 0, 7, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 0, 0, 0, 0, 8, 9},
            {0, 0, 0, 0, 8, 9, 0, 0, 0},
            {0, 8, 9, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 8, 9, 0},
            {0, 0, 0, 8, 9, 0, 0, 0, 0},
            {8, 9, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 8, 9, 0, 0},
            {0, 0, 8, 9, 0, 0, 0, 0, 0},
            {9, 0, 0, 0, 0, 0, 0, 0, 8}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 0, 0, 0, 0, 0, 9},
            {0, 0, 0, 0, 0, 9, 0, 0, 0},
            {0, 0, 9, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 9, 0},
            {0, 0, 0, 0, 9, 0, 0, 0, 0},
            {0, 9, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 9, 0, 0},
            {0, 0, 0, 9, 0, 0, 0, 0, 0},
            {9, 0, 0, 0, 0, 0, 0, 0, 0}}),
        sudoku_puzzle_new((int[9][9]){
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0}})};

    for(int i = 0; i < 9; i++) {
        assertEquals(sudoku_solve_simple(&puzzles[i]), (i==0) ? true : false);
        GList *res = sudoku_solve_diverge(&puzzles[i]);
        assertEquals(g_list_length(res), i+1);
        g_list_free_full(res, (void (*)(void*))g_bytes_unref);
    }
}

TEST(diverge_returns_correct_sudokus) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_new((int[9][9]){
        {0, 0, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 0, 0, 3},
        {7, 8, 9, 0, 0, 3, 4, 5, 6},
        {0, 3, 4, 5, 6, 7, 8, 9, 0},
        {5, 6, 7, 8, 9, 0, 0, 3, 4},
        {8, 9, 0, 0, 3, 4, 5, 6, 7},
        {3, 4, 5, 6, 7, 8, 9, 0, 0},
        {6, 7, 8, 9, 0, 0, 3, 4, 5},
        {9, 0, 0, 3, 4, 5, 6, 7, 8}});

    assertEquals(sudoku_solve_simple(&puzzle), false);

    sudoku_puzzle_t solutions[] = {puzzle, puzzle};
    *(sudoku_puzzle_cell(&solutions[0],0,0)) = sudoku_cell_new((int[]){1,0});
    *(sudoku_puzzle_cell(&solutions[1],0,0)) = sudoku_cell_new((int[]){2,0});

    GList *div = sudoku_solve_diverge(&puzzle);
    assertEquals(g_list_length(div), 2);

    sudoku_puzzle_t got[] = {
        sudoku_puzzle_unpack(g_list_nth_data(div, 0)),
        sudoku_puzzle_unpack(g_list_nth_data(div, 1))};

    assertEquals(sudoku_puzzle_equals_strict(&got[0], &solutions[0]), true);
    assertEquals(sudoku_puzzle_equals_strict(&got[1], &solutions[1]), true);

    g_list_free_full(div, (void (*)(void*))g_bytes_unref);
}
