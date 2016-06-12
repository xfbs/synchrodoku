#include "cu/cu.h"

/* sudoku_cell.c */
TEST(cell_solved_works);
TEST(cell_solved_works_with_unsolvable_cell);
TEST(cell_solved_works_with_unsolved_cell);
TEST(cell_intersect_works);
TEST(cell_candidates_works);

/* sudoku_pack.c */
TEST(pack_works_on_empty);
TEST(pack_preserves_bits);

/* solver_solve.c */
TEST(solve_works_with_empty_sudoku);

/* solver_diverge.c */

/* sudoku test suites */
TEST_SUITE(sudoku_cell) {
    TEST_ADD(cell_solved_works),
    TEST_ADD(cell_solved_works_with_unsolvable_cell),
    TEST_ADD(cell_solved_works_with_unsolved_cell),
    TEST_ADD(cell_intersect_works),
    TEST_ADD(cell_candidates_works),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(sudoku_pack) {
    TEST_ADD(pack_works_on_empty),
    TEST_ADD(pack_preserves_bits),
    TEST_SUITE_CLOSURE
};

/* solver tests suites */
TEST_SUITE(solver_solve) {
    TEST_ADD(solve_works_with_empty_sudoku),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(solver_diverge) {
    TEST_SUITE_CLOSURE
};

/* test suites */
TEST_SUITES {
    TEST_SUITE_ADD(sudoku_cell),
    TEST_SUITE_ADD(sudoku_pack),
    TEST_SUITE_ADD(solver_solve),
    TEST_SUITE_ADD(solver_diverge),
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