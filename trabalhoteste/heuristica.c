#include "heuristica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
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

// Função para calcular o número de possibilidades para uma célula
int count_possibilities(int **grid, int size, int row, int col) {
    int count = 0;
    for (int num = 1; num <= size; num++) {
        if (is_valid(grid, size, row, col, num)) count++;
    }
    return count;
}

// Função para encontrar a célula com menos possibilidades (heurística MRV)
Cell find_best_cell(int **grid, int size) {
    Cell best_cell = {-1, -1, size + 1};
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (grid[row][col] == 0) {
                int possibilities = count_possibilities(grid, size, row, col);
                if (possibilities < best_cell.possibilities) {
                    best_cell.row = row;
                    best_cell.col = col;
                    best_cell.possibilities = possibilities;
                }
            }
        }
    }
    return best_cell;
}

// Função de backtracking usando a heurística MRV
int heuristic_solve(int **grid, int size) {
    Cell cell = find_best_cell(grid, size);
    if (cell.row == -1) return 1; // Sudoku resolvido

    for (int num = 1; num <= size; num++) {
        if (is_valid(grid, size, cell.row, cell.col, num)) {
            grid[cell.row][cell.col] = num;
            if (heuristic_solve(grid, size)) return 1;
            grid[cell.row][cell.col] = 0;
        }
    }
    return 0; // Sem solução
}

// Função para carregar múltiplos Sudokus do arquivo
int load_multiple_sudokus(const char *filename, int ****puzzles, int **sizes, int *puzzle_count) {
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
void save_multiple_sudokus(const char *filename, int ***puzzles, int *sizes, int puzzle_count) {
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
    double real_time = seconds + microseconds * 1e-6;

    double cpu_time = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    printf("Tempo de execução (tempo real): %.6f segundos\n", real_time);
    printf("Tempo de execução (tempo de CPU): %.6f segundos\n", cpu_time);
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
    load_multiple_sudokus(input_file, &puzzles, &sizes, &puzzle_count);

    for (int p = 0; p < puzzle_count; p++) {
        printf("Resolvendo Sudoku #%d de tamanho %dx%d com heurística...\n", p + 1, sizes[p], sizes[p]);

        // Medir tempo de resolução
        struct timeval start, end;
        clock_t cpu_start, cpu_end;

        gettimeofday(&start, NULL);
        cpu_start = clock();

        int solved = heuristic_solve(puzzles[p], sizes[p]);

        cpu_end = clock();
        gettimeofday(&end, NULL);

        if (!solved) {
            fprintf(stderr, "Sem solução para o Sudoku #%d.\n", p + 1);
        } else {
            measure_time(&start, &end, cpu_start, cpu_end);
        }
    }

    save_multiple_sudokus(output_file, puzzles, sizes, puzzle_count);

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