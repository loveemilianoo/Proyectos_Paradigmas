#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void comparador (int *L, int *win, int hiloI, int hiloJ){
    int idxI = hiloI - 1;
    int idxJ = hiloJ - 1;

    int valorI = L[idxI];
    int valorJ = L[idxJ];

    int perdedor;
    if (valorI > valorJ){
        perdedor = hiloI;
    } else {
        perdedor = hiloJ;
    }
    int idxPerdedor = perdedor - 1;

    #pragma omp critical
    {
        win [idxPerdedor] += 1;
        printf("[Hilo P%02d-P%02d] Fase 2 | L[%02d]=%02d vs L[%02d]=%02d -> Pierde P%02d\n",
               hiloI, hiloJ, idxI, valorI, idxJ, valorJ, perdedor);
    }
}

void trabajador (int *L, int *win, int *L_orde, int hiloI){
    int idxOrigen = hiloI - 1;

    int valor = L[idxOrigen];
    int rango = win [idxOrigen];

    #pragma opm critical
    {
        L_orde[rango] = valor;
        printf("[Hilo P%02d] Fase 3 | Valor %02d tiene rango %02d -> Se mueve al indice %02d\n",
               hiloI, valor, rango, rango);
    }
}

void ordenamiento (int *L, int n, int *L_ordena){
    int *win = (int *) calloc (n ,sizeof(int));

    #pragma omp parallel forñ schedule(dynamic)
    for (int i=0; i<n; i++){
        for (int j=i+1; j<=n; j++){
            comparador(L, win, i, j);
        }
    }

    #pragma omp parallel for schedule(dynamic)
    for (int i=0; i<n; i++){
        trabajador(L, win, L_ordena, i);
    }

    free(win);
}

int main(){
    int n = 32;

    int pool[100];
    for (int i = 0; i < 100; i++){
        pool[i] = i + 1;
    }
    for (int i = 99; i > 0; i--) {
        int j = rand() % (i + 1);  
        int tmp = pool[i];
        pool[i] = pool[j];
        pool[j] = tmp;
    }

    int *L = (int *)malloc(n * sizeof(int)); 
    int *L_ordenado = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        L[i] = pool[i];
    }

    printf("Lista original (%d elementos):\n", n);
    for (int i = 0; i < n; i++)
        printf("%d ", L[i]);
    printf("\n\n");

    printf("--- Iniciando Ordenamiento CRCW con OpenMP ---\n");

    double tiempoIn = omp_get_wtime();
    ordenamiento(L, n, L_ordenado);
    double tiempoFin = omp_get_wtime();

    printf("\n--- Finalizado ---\n");
    printf("Lista ordenada:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", L_ordenado[i]);
    printf("\n");

    printf("\nTiempo de ejecucion: %.6f segundos\n", tiempoFin - tiempoIn);

    free(L);
    free(L_ordenado);
    return 0;
}