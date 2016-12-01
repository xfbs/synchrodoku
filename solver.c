#include "solver.h"

response_t solve_sudoku(request_t *request) {
    if(request->type != REQUEST_TASK) {
        return response_error();
    }

    // unpack sudoku puzzle
    sudoku_puzzle_t sudoku = sudoku_puzzle_unpack(request->data);

    // check if we can solve this
    if(sudoku_solve_simple(&sudoku)) {
        return response_solution(sudoku_puzzle_pack(&sudoku), request->id);
    } else {
        return response_diverge(sudoku_solve_diverge(&sudoku), request->id);
    }
}

response_t solve_nothing(request_t *request) {
    return response_diverge(NULL, request->id);
}

response_t solve_easy(request_t *request) {
    const char data[] = {1, 2, 3, 4};
    size_t data_len = 4;
    return response_solution(g_bytes_new_static(data, data_len), request->id);
}
