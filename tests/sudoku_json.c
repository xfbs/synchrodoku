#include "helpers.h"

json_t *json;

TEST(json_works_on_empty) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();

    json = sudoku_puzzle_to_json(&puzzle);
    sudoku_puzzle_t unpacked = sudoku_puzzle_from_json(json);
    json_decref(json);

    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &unpacked), true);
}

/*
TEST(pack_preserves_bits) {
    sudoku_puzzle_t puzzle = sudoku_puzzle_empty();

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            for(int n = 1; n <= 9; n++) {
                *(sudoku_puzzle_cell(&puzzle, r, c)) = sudoku_cell_new((int[]){n, 0});

                sudoku_puzzle_pack(packed, &puzzle);
                sudoku_puzzle_t unpacked = sudoku_puzzle_unpack(packed);
                assertEquals(sudoku_puzzle_equals_strict(&puzzle, &unpacked), true);

                if(n == 9) {
                    *(sudoku_puzzle_cell(&puzzle, r, c)) = sudoku_cell_new((int[]){0});
                }
            }
        }
    }
}

TEST(pack_works_with_random) {
    sudoku_puzzle_t puzzle, unpacked;

    puzzle = sudoku_puzzle_new((int[9][9]){
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {2, 3, 6, 3, 0, 7, 7, 7, 5},
            {2, 9, 6, 3, 9, 5, 2, 3, 5},
            {7, 3, 6, 1, 4, 9, 5, 2, 1},
            {5, 2, 2, 3, 4, 9, 2, 3, 4},
            {2, 3, 6, 2, 0, 0, 7, 8, 5},
            {7, 1, 6, 5, 7, 8, 0, 8, 7},
            {2, 3, 6, 9, 0, 0, 0, 7, 0},
            {5, 1, 2, 9, 8, 5, 5, 8, 7}});

    sudoku_puzzle_pack(packed, &puzzle);
    unpacked = sudoku_puzzle_unpack(packed);
    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &unpacked), true);

    // modify puzzle slightly
    *(sudoku_puzzle_cell(&puzzle, 2, 2)) = sudoku_cell_new((int[]){3, 5, 9, 0});
    *(sudoku_puzzle_cell(&puzzle, 5, 3)) = sudoku_cell_new((int[]){8, 4, 0});

    sudoku_puzzle_pack(packed, &puzzle);
    unpacked = sudoku_puzzle_unpack(packed);
    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &unpacked), true);

    puzzle = sudoku_puzzle_new((int[9][9]){
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {2, 3, 2, 3, 0, 7, 7, 7, 5},
            {2, 9, 6, 3, 9, 5, 2, 3, 5},
            {7, 3, 6, 2, 4, 2, 5, 1, 1},
            {5, 2, 2, -1, 4, 9, 2, 3, 4},
            {2, 3, 6, 2, 2, 0, 7, 8, 5},
            {7, 1, 8, 8, 8, 8, -1, 2, 7},
            {2, 3, 6, 9, 2, 0, 2, 7, 0},
            {5, 1, 2, -1, 8, 6, 6, 8, 7}});

    sudoku_puzzle_pack(packed, &puzzle);
    unpacked = sudoku_puzzle_unpack(packed);
    assertEquals(sudoku_puzzle_equals_strict(&puzzle, &unpacked), true);
}
*/
