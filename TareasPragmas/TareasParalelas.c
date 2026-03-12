#include <omp.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

    double promedio = 0;
    int minimo = arreglo[0];
    int orBinario = 0;
    double mediaGeo = 0;

    double inicio = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            double suma =0;
            for (int i=0; i<n; i++){
                suma += arreglo[i];
            }
            promedio = suma / n;
        }

        #pragma omp section
        {
            int min = arreglo[0];
            for (int i=0; i<n; i++){
                if (arreglo[i]<min){
                    min = arreglo[i];
                }
            }
            minimo = min;
        }

        #pragma omp section
        {
            int resultado = 0;
            for (int i=0; i<n; i++){
                resultado |= arreglo[i];
            }
            orBinario = resultado;
        }

        #pragma omp section
        {
            double radicando = 1.0;
            for (int i=0; i<n; i++){
                radicando *= arreglo [i];
            }
            mediaGeo = pow(radicando,1.0/n);
        }
    }

    double fin = omp_get_wtime();

    printf("--- Resultados ---\n");
    printf("Promedio:         %.4f\n", promedio);
    printf("Minimo:           %d\n",   minimo);
    printf("OR Binario:       %d\n",   orBinario);
    printf("Media Geometrica: %.4f\n", mediaGeo);
    printf("\nTiempo: %.6f segundos\n", fin - inicio);

    return 0;
}