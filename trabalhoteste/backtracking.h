#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <sys/time.h>
#include <math.h>
#include <time.h>

#define EMPTY 'v'

typedef struct {
    int row;
    int col;
    int possibilities;
} Cell;

int is_valid(int **grid, int size, int row, int col, int num);
int solve_sudoku(int **grid, int size);
int load_sudokus(const char *filename, int ****puzzles, int **sizes, int *puzzle_count);
void save_sudokus(const char *filename, int ***puzzles, int *sizes, int puzzle_count);
void measure_time(struct timeval *start, struct timeval *end, clock_t cpu_start, clock_t cpu_end);

#endif