#include "backtracking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

// Função para verificar se o número é válido na célula
int is_valid(int **grid, int size, int row, int col, int num) {
    for (int x = 0; x < size; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return 0;
    }
    int subgrid_rows = (int)sqrt(size);
    int subgrid_cols = size / subgrid_rows;

    if (subgrid_rows * subgrid_cols != size) {
        fprintf(stderr, "Tamanho inválido para subgrade do Sudoku %dx%d.\n", size, size);
        exit(EXIT_FAILURE);
    }

    int start_row = row - row % subgrid_rows;
    int start_col = col - col % subgrid_cols;

    for (int i = 0; i < subgrid_rows; i++) {
        for (int j = 0; j < subgrid_cols; j++) {
            if (grid[start_row + i][start_col + j] == num) return 0;
        }
    }
    return 1;
}

// Função de backtracking para resolver o Sudoku
int solve_sudoku(int **grid, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= size; num++) {
                    if (is_valid(grid, size, row, col, num)) {
                        grid[row][col] = num;
                        if (solve_sudoku(grid, size)) return 1;
                        grid[row][col] = 0;
                    }
                }
                return 0; // Sem solução
            }
        }
    }
    return 1; // Solução encontrada
}

// Função para carregar múltiplos Sudokus do arquivo
int load_sudokus(const char *filename, int ****puzzles, int **sizes, int *puzzle_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    *puzzle_count = 0;
    *puzzles = NULL;
    *sizes = NULL;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue; // Ignora comentários e linhas vazias

        // Determina o tamanho da grade com base na contagem de números na linha
        int size = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ' ' && line[i] != '\n') size++;
        }

        *puzzles = realloc(*puzzles, (*puzzle_count + 1) * sizeof(int **));
        *sizes = realloc(*sizes, (*puzzle_count + 1) * sizeof(int));
        (*sizes)[*puzzle_count] = size;

        // Aloca memória para a grade
        (*puzzles)[*puzzle_count] = malloc(size * sizeof(int *));
        for (int i = 0; i < size; i++) {
            (*puzzles)[*puzzle_count][i] = malloc(size * sizeof(int));
        }

        // Preenche a grade
        for (int row = 0; row < size; row++) {
            for (int col = 0, idx = 0; col < size; col++, idx += 2) {
                (*puzzles)[*puzzle_count][row][col] = (line[idx] == EMPTY) ? 0 : line[idx] - '0';
            }
            fgets(line, sizeof(line), file);
        }

        (*puzzle_count)++;
    }

    fclose(file);
    return *puzzle_count;
}

// Função para salvar múltiplos Sudokus no arquivo
void save_sudokus(const char *filename, int ***puzzles, int *sizes, int puzzle_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo de saída");
        exit(EXIT_FAILURE);
    }

    for (int p = 0; p < puzzle_count; p++) {
        int size = sizes[p];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (puzzles[p][i][j] == 0) {
                    fprintf(file, "%c ", EMPTY);
                } else {
                    fprintf(file, "%d ", puzzles[p][i][j]);
                }
            }
            fprintf(file, "\n");
        }
        if (p < puzzle_count - 1) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

// Função para medir o tempo de execução
void measure_time(struct timeval *start, struct timeval *end, clock_t cpu_start, clock_t cpu_end) {
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    double wall_time = seconds + microseconds * 1e-6;

    double cpu_time = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    printf("Tempo de relógio (real): %.6f segundos\n", wall_time);
    printf("Tempo de CPU: %.6f segundos\n", cpu_time);
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int ***puzzles;
    int *sizes;
    int puzzle_count;
    struct timeval start, end;

    // Carrega múltiplos Sudokus
    load_sudokus(input_file, &puzzles, &sizes, &puzzle_count);

    for (int p = 0; p < puzzle_count; p++) {
        printf("Resolvendo Sudoku #%d de tamanho %dx%d com backtracking...\n", p + 1, sizes[p], sizes[p]);

        // Medir tempo de resolução
        clock_t cpu_start, cpu_end;
        struct timeval start, end;

        cpu_start = clock();
        gettimeofday(&start, NULL);

        if (!solve_sudoku(puzzles[p], sizes[p])) {
            fprintf(stderr, "Sem solução para o Sudoku #%d.\n", p + 1);
        }

        cpu_end = clock();
        gettimeofday(&end, NULL);

        measure_time(&start, &end, cpu_start, cpu_end);
    }

    save_sudokus(output_file, puzzles, sizes, puzzle_count);

    // Libera memória
    for (int p = 0; p < puzzle_count; p++) {
        for (int i = 0; i < sizes[p]; i++) {
            free(puzzles[p][i]);
        }
        free(puzzles[p]);
    }
    free(puzzles);
    free(sizes);

    printf("Todos os Sudokus resolvidos e salvos em '%s'.\n", output_file);

    return 0;
}