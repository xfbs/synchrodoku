#include "helpers.h"

TEST(solve_sudoku_does_nothing_on_shutdown_request) {
    request_t request = request_shutdown();

    response_t response = solve_sudoku(&request);

    assertEquals(response.type, RESPONSE_ERROR);

    request_unref(&request);
    response_unref(&response);
}

TEST(solve_sudoku_does_nothing_on_error_request) {
    request_t request = request_error();

    response_t response = solve_sudoku(&request);

    assertEquals(response.type, RESPONSE_ERROR);

    request_unref(&request);
    response_unref(&response);
}

TEST(solve_sudoku_works_with_solved_sudoku) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_new((int[9][9]){
            {0, 2, 3, 4, 5, 6, 0, 8, 9},
            {4, 0, 6, 7, 8, 9, 1, 2, 3},
            {7, 8, 0, 1, 2, 3, 4, 5, 6},
            {3, 4, 5, 0, 7, 8, 9, 1, 2},
            {6, 7, 8, 9, 0, 2, 3, 4, 5},
            {9, 1, 2, 3, 4, 0, 6, 7, 8},
            {2, 3, 4, 5, 6, 7, 8, 9, 1},
            {5, 6, 7, 8, 9, 1, 2, 0, 4},
            {8, 9, 1, 2, 3, 4, 5, 6, 0}});
    sudoku_puzzle_t solution = sudoku_puzzle_new((int[9][9]){
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {4, 5, 6, 7, 8, 9, 1, 2, 3},
            {7, 8, 9, 1, 2, 3, 4, 5, 6},
            {3, 4, 5, 6, 7, 8, 9, 1, 2},
            {6, 7, 8, 9, 1, 2, 3, 4, 5},
            {9, 1, 2, 3, 4, 5, 6, 7, 8},
            {2, 3, 4, 5, 6, 7, 8, 9, 1},
            {5, 6, 7, 8, 9, 1, 2, 3, 4},
            {8, 9, 1, 2, 3, 4, 5, 6, 7}});

    request_t request = request_task(sudoku_puzzle_pack(&puzzle), 934);
    response_t response = solve_sudoku(&request);

    assertEquals(request.id, response.id);
    assertEquals(response.type, RESPONSE_SOLUTION);
    assertNotEquals(response.data.solution, NULL);

    sudoku_puzzle_t got = sudoku_puzzle_unpack(response.data.solution);

    assertEquals(sudoku_puzzle_equals_strict(&got, &solution), true);

    request_unref(&request);
    response_unref(&response);
}

TEST(solve_sudoku_works_with_diverging_sudoku) {
}
