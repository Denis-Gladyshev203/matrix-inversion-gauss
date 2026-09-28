#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "numbers.h"

double* create_matrix(int n){
    double* A = (double*)malloc(n * n * sizeof(double));
    return A;
}

void fill_matrix(double* A, int n, int k){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            A[i * n + j] = f(k, n, i, j);
        }
    }
}

double* read_matrix(const char* filename, int n){
    FILE* f_in = fopen(filename, "r");
    if (!f_in){
        printf("Ошибка! Невозможно открыть файл!\n");
        return NULL;
    }

    double* A = create_matrix(n);
    if (!A){
        fclose(f_in);
        return NULL;
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (fscanf(f_in, "%lf", &A[i * n + j]) != 1){
                printf("Ошибка считывания информации из файла!\n");
                free(A);
                fclose(f_in);
                return NULL;
            }
        }
    }

    double trash;
    if (fscanf(f_in, "%lf", &trash) == 1){
        printf("Лишние данные в файле!\n");
        free(A);
        fclose(f_in);
        return NULL;
    }

    fclose(f_in);
    return A;
}

double* create_identity_matrix(int n){
    double* E = create_matrix(n);
    if (!E){
        return NULL;
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            E[i * n + j] = (i == j) ? 1.0 : 0.0;
        }
    }

    return E;
}

void print_matrix(const double* A, int rows, int cols, int max_N){
    if (!A){
        printf("Где матрица, бро?\n");
        return;
    }

    int p_rows = (rows < max_N) ? rows : max_N;
    int p_cols = (cols < max_N) ? cols : max_N;

    for (int i = 0; i < p_rows; i++){
        for (int j = 0; j < p_cols; j++){
            printf(" %10.3e", A[i * cols + j]);
        }
        printf("\n");
    }
}

int gauss_invert(double* A, double* A_inv, int* col_perm, double* tmp, int n){
    double eps = 1e-15;

    for (int i = 0; i < n; i++){
        col_perm[i] = i;
    }
    
    //инициализируем матрицу A_inv как единичную
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            A_inv[i * n + j] = (i == j) ? 1.0 : 0.0;
        }
    }
    for (int k = 0; k < n; k++){
        double max_element = fabs(A[k * n + k]);
        int max_col = k;
        for (int j = k + 1; j < n; j++){ //поиск максимального элемента в k строке
            if (fabs(A[k * n + j]) > max_element){
                max_element = fabs(A[k * n + j]);
                max_col = j;
            }
        }

        if (max_element < eps){
            printf("singular matrix!\n");
            return -1;
        }
        //меняем k и max_col столбцы
        if (max_col != k){
            for (int i = 0; i < n; i++){
                double tmp = A[i * n + k];
                A[i * n + k] = A[i * n + max_col];
                A[i * n + max_col] = tmp;
            }
            int tmp = col_perm[k];
            col_perm[k] = col_perm[max_col];
            col_perm[max_col] = tmp;
        }

        //нормируем нашу новую строку
        double norm_coeff = A[k * n + k];
        for (int j = 0; j < n; j++){
            A[k * n + j] /= norm_coeff;
            A_inv[k * n + j] /= norm_coeff;
        } 

        //вычитаем из i строки k
        for (int i = 0; i < n; i++){
            if (i != k){
                double value = A[i * n + k];
                if (fabs(value) < 1e-15){
                    continue;
                }

                int row_i = i * n;
                int row_k = k * n;

                for (int j = 0; j < n; j++){
                    A[row_i + j] -= value * A[row_k + j];
                    A_inv[row_i + j] -= value * A_inv[row_k + j];
                }
            }
        }
    }

    //восстановление порядка строк в обратной матрице
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            tmp[col_perm[i] * n + j] = A_inv[i * n + j];
        }
    }
    for (int i = 0; i < n * n; i++){
        A_inv[i] = tmp[i];
    }
    return 0;
}

double norm_of_the_diff(const double* A, const double* A_inv, int n){
    double max_row_sum = 0.0;

    for (int i = 0; i < n; i++){
        double row_sum = 0.0;
        int row_i = i * n;

        for (int j = 0; j < n; j++){
            double product = 0.0;

            for (int k = 0; k < n; k++){
                product += A[row_i + k] * A_inv[k * n + j];
            }
            if (i == j){
                product -= 1.0;
            }

            row_sum += fabs(product);
        }
        if (row_sum > max_row_sum){
            max_row_sum = row_sum;
        }
    }

    return max_row_sum;
}