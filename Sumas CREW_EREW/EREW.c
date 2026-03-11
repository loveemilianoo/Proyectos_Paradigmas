#include <omp.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void sumaEREW (int*arreglo, int n){
    int *datos = malloc(n*sizeof(int));
    int *buffer = malloc(n*sizeof(int));

    for (int i=0; i<n; i++){
        datos[i] = arreglo[i];
    }    

    int pasos = (int) ceil(log2(n));
    double inicio = omp_get_wtime();

    for (int paso=0; paso< pasos; paso++){
        int stride = 1 << paso;

        for (int i=0; i<n; i++){
            buffer[i] = datos[i];
        }

        int hilosActivos = 0;

        #pragma omp parallel for reduction(+:hilosActivos) schedule(static)
        for (int i=0; i<n; i+= stride*2){
            if (i+stride < n){
                datos[i] = buffer[i] +buffer[i+stride];
                hilosActivos ++;
            }
        }

        printf("Paso %d | %d hilos activos: [", paso+1, hilosActivos);
        for (int i=0; i<n; i++){
            printf(i<n-1 ? "%d, ": "%d",datos[i]);
        }
        printf("]\n");
    }

    double fin = omp_get_wtime();
    printf("Resultado: %d\n", datos[0]);
    printf("Tiempo: %.6f segundos\n", fin - inicio);

    free(datos);
    free(buffer);
}

int main(){
    int n = 32;
    int arreglo[n];
    srand(time(NULL));

    for (int i = 0; i < n; i++){
        arreglo[i] = rand() % 100 + 1;
    }

    printf("Arreglo (%d elementos): [", n);
    for (int i = 0; i < n; i++){
        printf(i < n - 1 ? "%d, " : "%d", arreglo[i]);
    }
    printf("]\n");

    sumaEREW(arreglo, n);
    return 0;
}
