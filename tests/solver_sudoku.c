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
