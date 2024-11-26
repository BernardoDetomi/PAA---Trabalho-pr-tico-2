#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#define SIZE 9
#define EMPTY 'v'

// Função para verificar se o número é válido na célula
int is_valid(int grid[SIZE][SIZE], int row, int col, int num) {
    // Verifica na linha
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num) return 0;
    }
    // Verifica na coluna
    for (int x = 0; x < SIZE; x++) {
        if (grid[x][col] == num) return 0;
    }
    // Verifica na subgrade 3x3
    int start_row = row - row % 3, start_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[start_row + i][start_col + j] == num) return 0;
        }
    }
    return 1;
}

// Função de backtracking para resolver o Sudoku
int solve_sudoku(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (is_valid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solve_sudoku(grid)) return 1;
                        grid[row][col] = 0;
                    }
                }
                return 0; // Sem solução
            }
        }
    }
    return 1; // Solução encontrada
}

// Função para carregar o Sudoku do arquivo
void load_sudoku(const char *filename, int grid[SIZE][SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de entrada");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char ch;
            fscanf(file, " %c", &ch);
            grid[i][j] = (ch == EMPTY) ? 0 : ch - '0';
        }
    }
    fclose(file);
}

// Função para salvar o Sudoku resolvido no arquivo
void save_sudoku(const char *filename, int grid[SIZE][SIZE]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo de saída");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                fprintf(file, "%c ", EMPTY);
            } else {
                fprintf(file, "%d ", grid[i][j]);
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// Função para medir o tempo de execução
void measure_time(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Tempo de execução: %.6f segundos\n", elapsed);
}

// Main
int main(int argc, char *argv[]) {
    char *input_file = NULL;
    char *output_file = NULL;
    int opt;

    // Parse dos argumentos
    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            default:
                fprintf(stderr, "Uso: %s -i <arquivo_entrada> -o <arquivo_saida>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!input_file || !output_file) {
        fprintf(stderr, "Arquivos de entrada e saída são obrigatórios.\n");
        exit(EXIT_FAILURE);
    }

    int grid[SIZE][SIZE];
    struct timeval start, end;

    // Carrega o Sudoku
    load_sudoku(input_file, grid);

    // Medir tempo de resolução
    gettimeofday(&start, NULL);
    if (!solve_sudoku(grid)) {
        fprintf(stderr, "Sem solução para o Sudoku fornecido.\n");
        exit(EXIT_FAILURE);
    }
    gettimeofday(&end, NULL);

    // Salvar o resultado
    save_sudoku(output_file, grid);

    // Mostrar tempo
    measure_time(&start, &end);

    return 0;
}