#include "helpers.h"

TEST(pack_works_on_empty) {
    sudoku_puzzle_t puzzle;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            for(int n = 0; n < 9; n++) {
                sudoku_puzzle_cell(&puzzle, r, c)->numbers[n] = false;
            }
        }
    }

    unsigned char packed[92];

    sudoku_puzzle_pack(packed, &puzzle);

    for(int i = 0; i < 92; i++) {
        assertEquals(packed[i], 0);
    }

    sudoku_puzzle_t unpacked = sudoku_puzzle_unpack(packed);

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            for(int n = 0; n < 9; n++) {
                assertEquals(sudoku_puzzle_cell(&puzzle, r, c)->numbers[n], false);
            }
        }
    }
}

TEST(pack_preserves_bits) {
    sudoku_puzzle_t puzzle;

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            for(int n = 0; n < 9; n++) {
                sudoku_puzzle_cell(&puzzle, r, c)->numbers[n] = false;
            }
        }
    }

    unsigned char packed[92];

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            for(int n = 0; n < 9; n++) {
                sudoku_puzzle_cell(&puzzle, r, c)->numbers[n] = true;
                sudoku_puzzle_pack(packed, &puzzle);

                sudoku_puzzle_t unpacked = sudoku_puzzle_unpack(packed);
                assertEquals(sudoku_puzzle_cell(&puzzle, r, c)->numbers[n], true);
                sudoku_puzzle_cell(&puzzle, r, c)->numbers[n] = false;
            }
        }
    }
}
