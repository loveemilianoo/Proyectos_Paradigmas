#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void intervalo(int *L, int *odd, int *even, int idx) {
    L[2*idx]     = odd[idx];
    L[2*idx + 1] = even[idx];
}

void comparador(int *L, int i) {
    int idxIzq = 2*i - 1;
    int idxDer = 2*i;

    if (L[idxIzq] > L[idxDer]) {
        int tmp    = L[idxIzq];
        L[idxIzq]  = L[idxDer];
        L[idxDer]  = tmp;

        #pragma omp critical
        printf("[Hilo P%02d-P%02d] Fase 2 | L[%02d]=%02d vs L[%02d]=%02d -> Intercambia\n",
               2*i, 2*i+1, idxIzq, L[idxIzq], idxDer, L[idxDer]);
    }
}

void oddEvenMerge(int *L, int n) {
    if (n == 2) {
        if (L[0] > L[1]) {
            int tmp = L[0];
            L[0]    = L[1];
            L[1]    = tmp;
            #pragma omp critical
            printf("[Hilo P02-P03] Fase 2 | L[00]=%02d vs L[01]=%02d -> Intercambia\n",
                   L[0], L[1]);
        }
        return;
    }

    int mitad = n / 2;
    int *odd  = (int *)malloc(mitad * sizeof(int));
    int *even = (int *)malloc(mitad * sizeof(int));

    for (int i = 0; i < mitad; i++) {
        odd[i]  = L[2*i];      
        even[i] = L[2*i + 1];
    }
    
    #pragma omp parallel sections
    {
        #pragma omp section
        oddEvenMerge(odd, mitad);

        #pragma omp section
        oddEvenMerge(even, mitad);
    }
    
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < mitad; i++) {
        intervalo(L, odd, even, i);
    }

    #pragma omp parallel for schedule(static)
    for (int i = 1; i < mitad; i++) {
        comparador(L, i);
    }

    free(odd);
    free(even);
}

void oddEvenSort(int *L, int n, int nivel) {
    if (n <= 1) return;

    int mitad = n / 2;

    int *izq = L;           
    int *der = L + mitad;  

    #pragma omp critical
    printf("Sort N%d Dividiendo arreglo de tamaño %d\n", nivel, n);

    #pragma omp parallel sections
    {
        #pragma omp section
        oddEvenSort(izq, mitad, nivel + 1);

        #pragma omp section
        oddEvenSort(der, mitad, nivel + 1);
    }

    #pragma omp critical
    printf("Sort N%d Fusionando arreglo de tamaño %d\n", nivel, n);

    oddEvenMerge(L, n);
}

int main() {
    int n = 32;
    srand((unsigned int)time(NULL));

    int *arreglo = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arreglo[i] = rand() % 100 + 1;

    printf("Arreglo original (%d elementos):\n", n);
    for (int i = 0; i < n; i++)
        printf("%d ", arreglo[i]);
    printf("\n\n");

    printf("--- Iniciando Ordenamiento Batcher Odd-Even Merge Sort con OpenMP ---\n");

    double t_inicio = omp_get_wtime();
    oddEvenSort(arreglo, n, 1);
    double t_fin = omp_get_wtime();

    printf("\n--- Ordenamiento Finalizado ---\n");
    printf("Arreglo ordenado:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", arreglo[i]);
    printf("\n");
    printf("\nTiempo total de ejecucion: %.6f segundos\n", t_fin - t_inicio);

    free(arreglo);
    return 0;
}