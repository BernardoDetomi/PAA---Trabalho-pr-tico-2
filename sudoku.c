#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#define SIZE 9
#define EMPTY 'v'

typedef struct {
    int row;
    int col;
    int possibilities;
} Cell;

// Verificar se o número é válido
int is_valid(int grid[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return 0;
    }
    int start_row = row - row % 3, start_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[start_row + i][start_col + j] == num) return 0;
        }
    }
    return 1;
}

// Backtracking simples
int solve_sudoku_simple(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (is_valid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solve_sudoku_simple(grid)) return 1;
                        grid[row][col] = 0;
                    }
                }
                return 0; // Sem solução
            }
        }
    }
    return 1; // Solução encontrada
}

// Contar possibilidades para uma célula (usada pelo MRV)
int count_possibilities(int grid[SIZE][SIZE], int row, int col) {
    int count = 0;
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid(grid, row, col, num)) count++;
    }
    return count;
}

// Encontra a célula com menos possibilidades (heurística MRV)
Cell find_best_cell(int grid[SIZE][SIZE]) {
    Cell best_cell = {-1, -1, SIZE + 1};
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                int possibilities = count_possibilities(grid, row, col);
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

// Backtracking com heurística MRV
int solve_sudoku_with_heuristic(int grid[SIZE][SIZE]) {
    Cell cell = find_best_cell(grid);
    if (cell.row == -1) return 1; // Sudoku resolvido

    for (int num = 1; num <= SIZE; num++) {
        if (is_valid(grid, cell.row, cell.col, num)) {
            grid[cell.row][cell.col] = num;
            if (solve_sudoku_with_heuristic(grid)) return 1;
            grid[cell.row][cell.col] = 0;
        }
    }
    return 0; // Sem solução
}

// Carregar Sudoku do arquivo
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

// Salvar Sudoku resolvido no arquivo
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

// Medir tempo de execução
double measure_time(int (*solver)(int grid[SIZE][SIZE]), int grid[SIZE][SIZE]) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    solver(grid);
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    return seconds + microseconds * 1e-6;
}

// Main
int main(int argc, char *argv[]) {
    char *input_file = NULL;
    char *output_file = NULL;
    int method = 0; // 0 = Simples, 1 = MRV
    int opt;

    while ((opt = getopt(argc, argv, "i:o:m:")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'm':
                method = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Uso: %s -i <arquivo_entrada> -o <arquivo_saida> -m <0=simples, 1=heurística>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!input_file || !output_file) {
        fprintf(stderr, "Arquivos de entrada e saída são obrigatórios.\n");
        exit(EXIT_FAILURE);
    }

    int grid[SIZE][SIZE], grid_copy[SIZE][SIZE];
    load_sudoku(input_file, grid);

    // Copiar o grid para uso em ambos os métodos
    memcpy(grid_copy, grid, sizeof(grid));

    if (method == 0) {
        double time = measure_time(solve_sudoku_simple, grid);
        printf("Tempo (backtracking simples): %.6f segundos\n", time);
    } else {
        double time = measure_time(solve_sudoku_with_heuristic, grid_copy);
        printf("Tempo (heurística MRV): %.6f segundos\n", time);
    }

    save_sudoku(output_file, (method == 0) ? grid : grid_copy);

    return 0;
}