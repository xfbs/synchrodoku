#include "cu/cu.h"

/* sudoku_cell.c */
TEST(cell_solved_works);
TEST(cell_solved_works_with_unsolvable_cell);
TEST(cell_solved_works_with_unsolved_cell);
TEST(cell_intersect_works);

TEST_SUITE(sudoku_cell) {
    TEST_ADD(cell_solved_works),
    TEST_ADD(cell_solved_works_with_unsolvable_cell),
    TEST_ADD(cell_solved_works_with_unsolved_cell),
    TEST_ADD(cell_intersect_works),
    TEST_SUITE_CLOSURE
};

/* test suites */
TEST_SUITES {
    TEST_SUITE_ADD(sudoku_cell),
    TEST_SUITES_CLOSURE
};
int main(int argc, char *argv[])
{
    CU_SET_NAME("sudoku");
    CU_SET_OUT_PREFIX("output/");
    CU_RUN(argc, argv);

    // set return value according to whether
    // there were any failures
    return (cu_fail_test_suites > 0) ? -1 : 0;
}
