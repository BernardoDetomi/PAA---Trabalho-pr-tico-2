#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <dirent.h>

#define SIZE 9
#define EMPTY 'v'
#define MAX_PATH_LENGTH 512


typedef struct {
    int row;           
    int col;           
    int possibilities;  
} Cell;

int is_valid(int grid[SIZE][SIZE], int row, int col, int num);

int solve_sudoku_simple(int grid[SIZE][SIZE]);

int count_possibilities(int grid[SIZE][SIZE], int row, int col);

Cell find_best_cell(int grid[SIZE][SIZE]);

int solve_sudoku_with_heuristic(int grid[SIZE][SIZE]);

void load_sudoku(const char *filename, int grid[SIZE][SIZE]);

void save_sudoku(const char *filename, int grid[SIZE][SIZE]);

double measure_time(int (*solver)(int grid[SIZE][SIZE]), int grid[SIZE][SIZE]);

void process_multiple_sudokus(const char *dir_name, const char *output_file);

#endif