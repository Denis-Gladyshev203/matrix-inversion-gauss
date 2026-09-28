#ifndef MATRIX_H
#define MATRIX_H

double* create_matrix(int n);
void fill_matrix(double* A, int n, int k);
double* read_matrix(const char* filename, int n);
double* create_identity_matrix(int n);
void print_matrix(const double* A, int rows, int cols, int max_N);
double norm_of_the_diff(const double* A, const double* A_inv, int n);
int gauss_invert(double* A, double* A_inv, int* col_perm, double* tmp, int n);

#endif