#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

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
int count_possibilities(int **grid, int size, int row, int col);
Cell find_best_cell(int **grid, int size);
int heuristic_solve(int **grid, int size);
int backtracking_solve(int **grid, int size);
int load_multiple_sudokus(const char *filename, int ****puzzles, int **sizes, int *puzzle_count);
void save_multiple_sudokus(const char *filename, int ***puzzles, int *sizes, int puzzle_count);
void measure_time(struct timeval *start, struct timeval *end, clock_t cpu_start, clock_t cpu_end); // Corrigido

#endif