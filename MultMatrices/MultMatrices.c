#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#define IDX(i,j,k,n) ((i)*(n+1)*(n+1)+(j)*(n+1)+(k))

void matMultCRCW (double *A, double *B, double *resultado, int n){
    int tam = (n+1) * (n+1) * (n+1);
    double *C = (double *) calloc (tam, sizeof(double));

    #pragma omp parallel for schedule(dynamic) collapse(3)
    for (int i=1; i<=n; i++){
        for (int j=1; j<=n; j++){
            for (int k=1; k<=n; k++){
                C[IDX(i,j,k,n)] = A[(i-1)*n + (k-1)] * B[(k-1)*n + (j-1)];
            }
        }
    }
    
    #pragma omp parallel for schedule(dynamic) collapse(2)
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            double suma = 0.0;
            for (int k = 1; k <= n; k++) {
                suma += C[IDX(i,j,k,n)];
            }
            resultado[(i-1)*n + (j-1)] = suma;
        }
    }

    free(C);
}

void imprimir_matriz(const char *nombre, double *M, int n) {
    printf("\n%s:\n", nombre);
    for (int i = 0; i < n; i++) {
        printf("  ");
        for (int j = 0; j < n; j++)
            printf("%8.2f  ", M[i*n + j]);
        printf("\n");
    }
}

int main() {
    int n = 4;
    srand((unsigned int)time(NULL));

    double *A       = (double *)malloc(n * n * sizeof(double));
    double *B       = (double *)malloc(n * n * sizeof(double));
    double *result  = (double *)malloc(n * n * sizeof(double));

    for (int i = 0; i < n * n; i++) {
        A[i] = rand() % 10;   
        B[i] = rand() % 10;
    }

    imprimir_matriz("Matriz A", A, n);
    imprimir_matriz("Matriz B", B, n);

    printf("\n--- Ejecutando MatMultCRCW con OpenMP ---\n");

    double t0 = omp_get_wtime();
    matMultCRCW(A, B, result, n);
    double t1 = omp_get_wtime();

    imprimir_matriz("Resultado de las matrices", result, n);
    printf("\n  Tiempo CRCW: %.4f s\n", t1 - t0);

    free(A);
    free(B);
    free(result);

    return 0;
}