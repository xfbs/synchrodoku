#include "helpers.h"

TEST(cell_solved_works) {
    for(int i = 0; i < 9; i++) {
        sudoku_cell_t cell;

        for(int n = 0; n < 9; n++) {
            cell.numbers[n] = false;
        }

        cell.numbers[i] = true;

        assertEquals(sudoku_cell_solved(&cell), true);
        assertEquals(sudoku_cell_solvable(&cell), true);
        assertEquals(sudoku_cell_solution(&cell), i+1);
    }
}

TEST(cell_solved_works_with_unsolvable_cell) {
    sudoku_cell_t cell;

    for(int n = 0; n < 9; n++) {
        cell.numbers[n] = false;
    }

    assertEquals(sudoku_cell_solved(&cell), false);
    assertEquals(sudoku_cell_solvable(&cell), false);
    assertEquals(sudoku_cell_solution(&cell), 0);
}

TEST(cell_solved_works_with_unsolved_cell) {
    sudoku_cell_t cell;

    for(int n = 0; n < 9; n++) cell.numbers[n] = false;
    cell.numbers[2] = true;
    cell.numbers[8] = true;

    assertEquals(sudoku_cell_solved(&cell), false);
    assertEquals(sudoku_cell_solvable(&cell), true);
    assertEquals(sudoku_cell_solution(&cell), 0);
}

TEST(cell_intersect_works) {
    sudoku_cell_t a, b, e, i;

    a = sudoku_cell_new((int[]) {1, 2, 3, 0});
    b = sudoku_cell_new((int[]) {4, 5, 6, 7, 8, 9, 0});
    e = sudoku_cell_new((int[]) {0});
    i = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_equals_strict(&e, &i), true);

    a = sudoku_cell_new((int[]) {1, 2, 3, 0});
    b = sudoku_cell_new((int[]) {3, 4, 5, 6, 7, 8, 9, 0});
    e = sudoku_cell_new((int[]) {3, 0});
    i = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_equals_strict(&e, &i), true);

    a = sudoku_cell_new((int[]) {1, 2, 3, 7, 0});
    b = sudoku_cell_new((int[]) {4, 5, 6, 7, 8, 9, 0});
    e = sudoku_cell_new((int[]) {7, 0});
    i = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_equals_strict(&e, &i), true);

    a = sudoku_cell_new((int[]) {1, 2, 0});
    b = sudoku_cell_new((int[]) {4, 2, 0});
    e = sudoku_cell_new((int[]) {2, 0});
    i = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_equals_strict(&e, &i), true);

    a = sudoku_cell_new((int[]) {1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    b = sudoku_cell_new((int[]) {4, 2, 0});
    e = sudoku_cell_new((int[]) {4, 2, 0});
    i = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_equals_strict(&e, &i), true);

    a = sudoku_cell_new((int[]) {1, 4, 5, 6, 7, 8, 9, 0});
    b = sudoku_cell_new((int[]) {4, 2, 0});
    e = sudoku_cell_new((int[]) {4, 0});
    i = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_equals_strict(&e, &i), true);
}

TEST(cell_candidates_works) {
    sudoku_cell_t cell;

    cell = sudoku_cell_new((int[]) {1, 4, 5, 0});
    assertEquals(sudoku_cell_candidates(&cell), 3);

    cell = sudoku_cell_new((int[]) {1, 2, 3, 4, 5, 6, 7, 8, 0});
    assertEquals(sudoku_cell_candidates(&cell), 8);

    cell = sudoku_cell_new((int[]) {5, 7, 9, 2, 0});
    assertEquals(sudoku_cell_candidates(&cell), 4);

    cell = sudoku_cell_new((int[]) {1, 0});
    assertEquals(sudoku_cell_candidates(&cell), 1);

    cell = sudoku_cell_new((int[]) {0});
    assertEquals(sudoku_cell_candidates(&cell), 0);

}

TEST(cell_constructors_work) {
    sudoku_cell_t cell = sudoku_cell_empty();

    for(int n = 0; n < 9; n++) {
        assertEquals(cell.numbers[n], false);
    }

    cell = sudoku_cell_any();

    for(int n = 0; n < 9; n++) {
        assertEquals(cell.numbers[n], true);
    }

    cell = sudoku_cell_new((int[]){1, 3, 5, 7, 9, 0});

    for(int n = 0; n < 9; n+=2) {
        assertEquals(cell.numbers[n], true);
    }

    for(int n = 1; n < 9; n+=2) {
        assertEquals(cell.numbers[n], false);
    }

    cell = sudoku_cell_new((int[]){2, 4, 6, 8, 0});

    for(int n = 0; n < 9; n+=2) {
        assertEquals(cell.numbers[n], false);
    }

    for(int n = 1; n < 9; n+=2) {
        assertEquals(cell.numbers[n], true);
    }
}
