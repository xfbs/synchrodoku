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
    sudoku_cell_t a, b;

    for(int n = 0; n < 9; n++) {
        a.numbers[n] = false;
        b.numbers[n] = false;
    }

    sudoku_cell_t res;

    res = sudoku_cell_intersect(&a, &b);
    for(int n = 0; n < 9; n++) assertEquals(res.numbers[n], false);

    a.numbers[4] = true;

    res = sudoku_cell_intersect(&a, &b);
    for(int n = 0; n < 9; n++) assertEquals(res.numbers[n], false);

    b.numbers[4] = true;
    res = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_solution(&res), 5);

    b.numbers[8] = true;
    res = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_solution(&res), 5);

    a.numbers[8] = true;
    res = sudoku_cell_intersect(&a, &b);
    assertEquals(sudoku_cell_solvable(&res), true);
    assertEquals(sudoku_cell_solution(&res), 0);
}

TEST(cell_candidates_works) {
    sudoku_cell_t cell = sudoku_cell_empty();

    for(int i = 0; i < 9; i++) {
        assertEquals(sudoku_cell_candidates(&cell), i);
        cell.numbers[i] = true;
    }

    cell = sudoku_cell_empty();
    cell.numbers[3] = true;
    cell.numbers[6] = true;
    assertEquals(sudoku_cell_candidates(&cell), 2);
    cell.numbers[8] = true;
    assertEquals(sudoku_cell_candidates(&cell), 3);
}
