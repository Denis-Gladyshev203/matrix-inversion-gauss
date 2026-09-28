#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

int main(int argc, char* argv[]){
    if (argc < 4){
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int k = atoi(argv[3]);
    const char* filename = (k == 0 && argc > 4) ? argv[4] : NULL;

    if (n <= 0 || m < 0 || k < 0 || k > 4){
        printf("Uncorrect values!\n");
        return 1;
    }

    if (k == 0 && !filename){
        printf("No filename when k = 0!");
        return 1;
    }

    //подготовка к алгоритму
    double* A = create_matrix(n);
    double* A_inv = create_matrix(n);
    int* col_perm = (int*)malloc(n * sizeof(int));
    double* tmp = create_matrix(n);

    if (!A || !A_inv || !col_perm || !tmp){
        printf("Memory error!\n");
        if (A){ free(A); }
        if (A_inv){ free(A_inv); }
        if (col_perm){ free(col_perm); }
        if (tmp){ free(tmp); }
        return 1;
    }

    if (k != 0){
        fill_matrix(A, n, k);
    }
    else{
        double* tmp_matrix = read_matrix(filename, n);
        if (!tmp){
            free(A);
            free(A_inv);
            free(col_perm);
            free(tmp);
            return 1;
        }
        for (int i = 0; i < n * n; i++){
            A[i] = tmp[i];
        }
        free(tmp_matrix);
    }

    printf("------ Original matrix ------\n");
    print_matrix(A, n, n, m);

    //алгоритм
    clock_t start = clock();
    int res = gauss_invert(A, A_inv, col_perm, tmp, n);
    clock_t end = clock();
    
    if (res != 0){
        free(A);
        free(A_inv);
        free(col_perm);
        free(tmp);
        return 1;
    }
    
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n------ Inverse matrix ------\n");
    print_matrix(A_inv, n, n, m);
    printf("\n");
    printf("\nAlgorithm execution time: %5.5f sec\n", time_spent);

    if (k != 0 ){
        fill_matrix(A, n, k);
    }
    else{
        double* tmp = read_matrix(filename, n);
        for (int i = 0; i < n * n; i++){
            A[i] = tmp[i];
        }
        free(tmp);
    }

    double norm = norm_of_the_diff(A, A_inv, n);
    printf("Discrepancy norm: %10.3e\n", norm);

    free(A);
    free(A_inv);
    free(col_perm);
    free(tmp);
    return 0;
}