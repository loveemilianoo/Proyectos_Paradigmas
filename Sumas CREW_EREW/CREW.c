#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));  
    int n = 32;
    int *arreglo = (int *)malloc(n * sizeof(int));
    int suma = 0;

    for (int i = 0; i < n; i++) {
        arreglo[i] = rand() % 100;  
    }
     
    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i<n; i++){
        suma += arreglo[i];
        printf("Hilo %d sumando arreglo[%d] = %d\n", omp_get_thread_num(), i, arreglo[i]);
    }

    printf("Suma total: %d\n", suma);
    free(arreglo);  
    return 0;        
}