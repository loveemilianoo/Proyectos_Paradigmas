#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

int minCRCW (int *L, int n){
    int *win = malloc(n* sizeof(int));

    #pragma omp parallel for schedule(static)
    for (int i=0; i<n; i++){
        win[i] =0;
    }

    #pragma omp parallel for schedule(static)
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if (i<j){
                if (L[i] > L[j]){
                    win[i] = 1;
                } else if (L[i] < L[j]){
                    win[j] = 1;
                }
            }
        }
    }
    
    int resultado = -1;

    #pragma omp parallel for schedule(static)
    for (int i=0; i<n; i++){
        if (win[i] == 0){
            resultado = L[i];
        }
    }

    free(win);
    return resultado;
}

int main() {
    int n = 32;
    int *arreglo = malloc(n * sizeof(int));
    if (!arreglo) { perror("malloc"); return EXIT_FAILURE; }

    // n valores únicos del 1 al 999
    int pool[999];
    for (int i = 0; i < 999; i++) pool[i] = i + 1;

    srand(time(NULL));
    // Mezclar TODO el pool primero
    for (int i = 998; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = pool[i]; pool[i] = pool[j]; pool[j] = tmp;
    }
    // Tomar los primeros n elementos ya mezclados
    for (int i = 0; i < n; i++){
        arreglo[i] = pool[i];
    }

    printf("BÚSQUEDA CRCW EN MODELO DE PARALELISMO\n");
    printf("Lista: [");
    for (int i = 0; i < n; i++){
        printf(i < n - 1 ? "%d, " : "%d", arreglo[i]);
    }
    printf("]\n");

    double inicio = omp_get_wtime();
    int resultado = minCRCW(arreglo, n);
    printf("\nTiempo de ejecucion: %.6f segundos\n", omp_get_wtime() - inicio);
    printf("El minimo es: %d\n", resultado);

    free(arreglo);
    return 0;
}