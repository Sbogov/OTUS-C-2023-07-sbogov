#include <stdlib.h>
#include <stdio.h>

double* init_array(int n_numbers, char** numbers) {
    double* massiv = malloc(sizeof(double) * n_numbers);
    //TODO Check
    for (int i = 0; i < n_numbers; i++) {
        massiv[i] = atof(numbers[i]);
        //TODO Check and ???
    }
    return massiv;
}

void de_init_array(double* massiv) {
    free(massiv);
}


double summa(int n,double* elem) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
         sum += elem[i];
        }
        printf("summa = %f\n", sum);
     return 0;
}

double multiplication(int n, double* elem){
    double multpl = 1;
    for (int i = 0; i < n; i++) {
        multpl *= elem[i];
    }
    printf("multiplication = %f\n", multpl);
    return 0;
}

double arithmetic_mean(int n, double* elem) {
    double sredn = 0;
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += elem[i];
    sredn = sum / n;
    printf("arithmetic mean = %f", sredn);
    return 0;
}

int run_main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Необходимо ввести больше одного элемента\n");
        return -1;
    }
    double* mass = init_array((argc - 1), &argv[1]);
    if (!mass) {
        return -1;
    }
    summa ((argc-1), mass);
    multiplication ((argc - 1), mass);
    arithmetic_mean ((argc - 1), mass);
    de_init_array (mass);
    return 0;
}

int main(int argc, char* argv[]) {
    argc = 3;
    char* args[3] = {argv[0], "3", "5"};
    int result = run_main(argc, args);
    if (result) {
        printf("run_main failed - %d\n", result);
        return result;
    };
    return 0;
}