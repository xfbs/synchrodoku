#include "cu/cu.h"

/* sudoku_cell.c */
TEST(cell_solved_works);
TEST(cell_solved_works_with_unsolvable_cell);
TEST(cell_solved_works_with_unsolved_cell);
TEST(cell_intersect_works);
TEST(cell_candidates_works);
TEST(cell_constructors_work);

/* sudoku_puzzle.c */
TEST(puzzle_new_works_with_full_sudoku);
TEST(puzzle_new_works_with_empty_sudoku);
TEST(puzzle_empty_works);
TEST(puzzle_candidates_row_works);
TEST(puzzle_candidates_col_works);
TEST(puzzle_candidates_square_works);

/* sudoku_pack.c */
TEST(pack_works_on_empty);
TEST(pack_preserves_bits);
TEST(pack_works_with_random);

/* sudoku_json.c */
TEST(json_works_on_empty);
TEST(json_preserves_bits);
TEST(json_works_with_random);

/* solver_solve.c */
TEST(solve_works_with_empty_sudoku);
TEST(solve_works_with_solved_sudoku);
TEST(solve_works_with_partially_solved_sudoku);
TEST(solve_stops_on_diverge);

/* solver_diverge.c */
TEST(diverge_returns_solved_sudoku);
TEST(diverge_returns_correct_amount);
TEST(diverge_returns_correct_sudokus);

/* request_new.c */
TEST(request_error_works);
TEST(request_shutdown_works);
TEST(request_task_works);

/* request_parse.c */
TEST(parsing_empty_request_yields_error);
TEST(parsing_malformed_request_yields_error);
TEST(parsing_shutdown_request_works);
TEST(parsing_task_request_works);

/* request_create.c */
TEST(request_create_does_not_work_with_error);
TEST(request_create_works_with_shutdown);
TEST(request_create_works_with_task);

/* response_new.c */
TEST(response_error_works);
TEST(response_solution_works);
TEST(response_diverge_works);

/* response_parse.c */
TEST(parsing_empty_response_yields_error);
TEST(parsing_malformed_response_yields_error);
TEST(parsing_solution_response_works);

/* sudoku test suites */
TEST_SUITE(sudoku_cell) {
    TEST_ADD(cell_solved_works),
    TEST_ADD(cell_solved_works_with_unsolvable_cell),
    TEST_ADD(cell_solved_works_with_unsolved_cell),
    TEST_ADD(cell_intersect_works),
    TEST_ADD(cell_candidates_works),
    TEST_ADD(cell_constructors_work),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(sudoku_puzzle) {
    TEST_ADD(puzzle_new_works_with_full_sudoku),
    TEST_ADD(puzzle_new_works_with_empty_sudoku),
    TEST_ADD(puzzle_empty_works),
    TEST_ADD(puzzle_candidates_row_works),
    TEST_ADD(puzzle_candidates_col_works),
    TEST_ADD(puzzle_candidates_square_works),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(sudoku_pack) {
    TEST_ADD(pack_works_on_empty),
    TEST_ADD(pack_preserves_bits),
    TEST_ADD(pack_works_with_random),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(sudoku_json) {
    TEST_ADD(json_works_on_empty),
    TEST_ADD(json_preserves_bits),
    TEST_ADD(json_works_with_random),
    TEST_SUITE_CLOSURE
};

/* solver tests suites */
TEST_SUITE(solver_solve) {
    TEST_ADD(solve_works_with_empty_sudoku),
    TEST_ADD(solve_works_with_solved_sudoku),
    TEST_ADD(solve_works_with_partially_solved_sudoku),
    TEST_ADD(solve_stops_on_diverge),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(solver_diverge) {
    TEST_ADD(diverge_returns_solved_sudoku),
    TEST_ADD(diverge_returns_correct_amount),
    TEST_ADD(diverge_returns_correct_sudokus),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(request_new) {
    TEST_ADD(request_error_works),
    TEST_ADD(request_shutdown_works),
    TEST_ADD(request_task_works),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(request_parse) {
    TEST_ADD(parsing_empty_request_yields_error),
    TEST_ADD(parsing_malformed_request_yields_error),
    TEST_ADD(parsing_shutdown_request_works),
    TEST_ADD(parsing_task_request_works),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(request_create) {
    TEST_ADD(request_create_does_not_work_with_error),
    TEST_ADD(request_create_works_with_shutdown),
    TEST_ADD(request_create_works_with_task),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(response_new) {
    TEST_ADD(response_error_works),
    TEST_ADD(response_solution_works),
    TEST_ADD(response_diverge_works),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(response_parse) {
    TEST_ADD(parsing_empty_response_yields_error),
    TEST_ADD(parsing_malformed_response_yields_error),
    TEST_ADD(parsing_solution_response_works),
    TEST_SUITE_CLOSURE
};


/* test suites */
TEST_SUITES {
    TEST_SUITE_ADD(sudoku_cell),
    TEST_SUITE_ADD(sudoku_puzzle),
    TEST_SUITE_ADD(sudoku_pack),
    TEST_SUITE_ADD(sudoku_json),
    TEST_SUITE_ADD(solver_solve),
    TEST_SUITE_ADD(solver_diverge),
    TEST_SUITE_ADD(request_new),
    TEST_SUITE_ADD(request_parse),
    TEST_SUITE_ADD(request_create),
    TEST_SUITE_ADD(response_new),
    TEST_SUITE_ADD(response_parse),
    TEST_SUITES_CLOSURE
};

int main(int argc, char *argv[])
{
    CU_SET_NAME("synchrodoku");
    CU_SET_OUT_PREFIX("tests/output/");
    CU_RUN(argc, argv);

    // set return value according to whether
    // there were any failures
    return (cu_fail_test_suites > 0) ? -1 : 0;
}
