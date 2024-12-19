#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <sys/time.h>

#define SIZE 9
#define EMPTY 'v'
#define MAX_PUZZLES 100

int is_valid(int grid[SIZE][SIZE], int row, int col, int num);
int solve_sudoku(int grid[SIZE][SIZE]);
int load_sudokus(const char *filename, int puzzles[MAX_PUZZLES][SIZE][SIZE]);
void save_sudokus(const char *filename, int puzzles[MAX_PUZZLES][SIZE][SIZE], int puzzle_count);
void measure_time(struct timeval *start, struct timeval *end);

#endif
