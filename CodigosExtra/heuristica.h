#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <sys/time.h>

#define SIZE 15
#define EMPTY 'v'

typedef struct {
    int row;
    int col;
    int possibilities;
} Cell;

int is_valid(int grid[SIZE][SIZE], int row, int col, int num);
int count_possibilities(int grid[SIZE][SIZE], int row, int col);
Cell find_best_cell(int grid[SIZE][SIZE]);
int heuristic_solve(int grid[SIZE][SIZE]);
int backtracking_solve(int grid[SIZE][SIZE]);
int load_multiple_sudokus(const char *filename, int puzzles[][SIZE][SIZE]);
void save_multiple_sudokus(const char *filename, int puzzles[][SIZE][SIZE], int puzzle_count);
void measure_time(struct timeval *start, struct timeval *end);


#endif