#include "cu/cu.h"

/* solver_solve.c */
TEST(solve_works_with_empty_sudoku);

/* solver_diverge.c */

TEST_SUITE(solver_solve) {
    TEST_ADD(solve_works_with_empty_sudoku),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(solver_diverge) {
    TEST_SUITE_CLOSURE
};

/* test suites */
TEST_SUITES {
    TEST_SUITE_ADD(solver_solve),
    TEST_SUITE_ADD(solver_diverge),
    TEST_SUITES_CLOSURE
};
int main(int argc, char *argv[])
{
    CU_SET_NAME("solver");
    CU_SET_OUT_PREFIX("output/");
    CU_RUN(argc, argv);

    // set return value according to whether
    // there were any failures
    return (cu_fail_test_suites > 0) ? -1 : 0;
}
