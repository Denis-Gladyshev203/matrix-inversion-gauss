#include <stdio.h>
#include <math.h>
#include "numbers.h"

static int get_max(int a, int b){
    return a > b ? a : b;
}

double f(int k, int n, int i, int j){
    int I = i + 1;
    int J = j + 1;

    if (k == 1){
        return n - get_max(I, J) + 1;
    }
    else if (k == 2){
        return get_max(I, J);
    }
    else if (k == 3){
        return fabs((double)(I - J));
    }
    else if (k == 4){
        return 1.0 / (I + J - 1);
    }
    return 0.0;
}