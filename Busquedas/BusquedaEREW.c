#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#define INF INT_MAX

void broadcast (int *A, int X, int n){
    double inicio = omp_get_wtime();

    A[1] = X;
    int k= (int)log2(n);

    for (int i=1; i<k; i++){
        int ini = (1 << (i-1)) +1;
        int fin = (1 << i);
        int paso = (1 << (i-1));

        int *buf = malloc((n + 1) * sizeof(int));
        for (int x = 1; x <= n; x++) buf[x] = A[x];

        #pragma omp parallel for schedule(static)
        for (int j= ini; j<=fin; j++){
            if (j<=n){
                A[j] = buf[j-paso];
            }
        }
        free(buf);
    }
    printf("Tiempo broadcast:   %.6f segundos\n", omp_get_wtime()-inicio);
}

int minimo (int *L, int n){
    double inicio = omp_get_wtime();
    int rondas = (int)log2(n);

    for (int j=1; j<=rondas; j++){
        int cantPares = n/(1<<j);
        int *buffer = malloc((cantPares+1)*sizeof(int));

        #pragma omp parallel for schedule(static)
        for (int i=1; i<=cantPares; i++){
            int izq = 2*i-1;
            int der = 2*i;
            buffer[i] = (L[izq]<L[der]) ? L[izq] : L[der];
        }

        for (int i=1; i<= cantPares; i++){
            L[i] = buffer[i];
        }
        free (buffer);
    }
    printf("Tiempo minimo: %.6f segundos\n", omp_get_wtime()-inicio);
    return L[1];
}

int busquedaEREW (int *L, int X, int n){
    double inicio = omp_get_wtime();

    int *Temp = malloc((n+1) * sizeof(int));

    broadcast(Temp, X, n);

    #pragma omp parallel for schedule(static)
    for (int i=1; i<=n; i++){
        Temp[i] = (L[i] == X) ? i : INF;
    }
    printf("Tiempo Busqueda EREW: %.6f segundos\n", omp_get_wtime()-inicio);
    
    int resultado = minimo (Temp, n);
    free(Temp);
    return resultado;
}

int main(){
    int n = 32;
    int *arreglo = malloc((n + 1) * sizeof(int));
    if (!arreglo) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    for (int i = 1; i <= n; i++){
        arreglo[i] = rand() % 100 + 1;
    }

    printf("BÚSQUEDA EREW EN MODELO DE PARALELISMO\n");
    printf("Lista: [");
    for (int i = 1; i <= n; i++)
        printf(i < n ? "%d, " : "%d", arreglo[i]);
    printf("]\n");

    int X;
    printf("\n¿Qué número querés buscar? ");
    scanf("%d", &X);

    int resultado = busquedaEREW(arreglo, X, n);

    if (resultado == INF)
        printf("\nEl elemento %d NO fue encontrado.\n", X);
    else
        printf("\nEl elemento %d fue encontrado en la posición %d.\n", X, resultado);

    free(arreglo);
    return 0;
}