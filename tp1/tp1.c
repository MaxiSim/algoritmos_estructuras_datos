#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Determina si un número es primo.
 */
bool is_prime(int x){
    if (x == 0){
        return false;
    } 
    if (x == 1){
        return true;
    }
    for (int i = 2; i < x; i++){
        if (x % i == 0){
            return false;
        }
    }
    return true;
}

/*
 * Dado el volumen de un deposito D y el volumen de un producto V
 * la función calcula cuantos productos entran en el depósito.
 */
int storage_capacity(float d, float v){
    float vol = d/v;
    return (int)vol;
}

/*
 * Intercambia dos valores de enteros.
 */
void swap(int *x, int *y){
    int aux = *x;
    *x = *y;
    *y = aux;
    return;
}

/*
 * Devuelve el máximo de un arreglo de enteros.
 */
int array_max(const int *array, int length){
    int max = array[0];
    for (int i = 1; i < length; i++){
        if (array[i] > max){
            max = array[i];
        }
    }
    return max;
}

/*
 * Aplica la función a cada elemento de un arreglo de enteros.
 */
void array_map(int *array, int length, int f(int)){
    if (!array){
        return;
    }
    if (!f){
        return;
    }
    for (int i = 0; i < length; i++){
        array[i] = f(array[i]);
    }
    return;
}

/*
 * Copia un arreglo de enteros en memoria dinámica.
 * Si el arreglo es NULL devuelve NULL.
 */
int *copy_array(const int *array, int length){
    if (!array){
        return NULL;
    }
    int* copy = (int *) malloc (length * sizeof(int));
    if (!copy){
        return NULL;
    }
    for (size_t i = 0; i < length; ++i){
        copy[i] = array[i];
    }
    return copy;
}

/*
 * Hace bubble sort sobre un arreglo de enteros ascendentemente.
 * Si el arreglo es NULL, no hace nada.
 */
void bubble_sort(int *array, int length){
    if (!array){
        return;
    }
    for (int i = 0; i < length; i++){
        for (int j = 0; j < length - 1; j++){
            if (array[j] > array[j+1]){
                swap(&array[j], &array[j+1]);
            }
        }
    }
    return;
}

/*
 * Determina si dos arreglos de enteros son identicamente iguales.
 * En el caso de que alguno sea NULL solo devuelve true si el otro tambien lo es.
 */
bool array_equal(const int *array1, int length1,
                 const int *array2, int length2){
    int contador = 0;
    if (!array1 && !array2){
        return true;
    } else if (!array1 || !array2){
        return NULL;
    }
    if (length1 != length2){
        return false;
    } else {
        for (int i = 0; i < length1; i++) {
            if (array1[i] != array2[i]){
                contador++;
            }
        }
        if (contador != 0){
            return false;
        }
    }
    return true;
}

/*
 * Determina si dos arrays de enteros son análogos a un anagrama para textos (en algun orden particular, son el mismo arreglo).
 * Si alguno es NULL devuelve false.
 */
bool integer_anagrams(const int *array1, int length1,
                      const int *array2, int length2){
    if (!array1 || !array2){
        return false;
    }
    if (length1 != length2){
        return false;
    }
    int * copy1 = copy_array(array1, length1);
    int * copy2 = copy_array(array2, length2);
    bubble_sort(copy1, length1);
    bubble_sort(copy2, length2);

    if (!array_equal(copy1, length1, copy2, length2)){
        free(copy1);
        free(copy2);
        return false;
    }
    
    free(copy1);
    free(copy2);
    return true;
}

/*
 * Copia un arreglo de arreglos de enteros en memoria dinámica.
 * Si alguno de ellos en NULL, continua siendo NULL.
 * Si el arreglo de arreglos es NULL, devuelve NULL.
 *
 * array_of_arrays: un arreglo de punteros a arreglos de enteros
 * array_lenghts: un arreglo con los largos de cada arreglo en array_of_arrays
 * array_amount: la cantidad de arreglos
 */
int **copy_array_of_arrays(const int **array_of_arrays, const int *array_lenghts, int array_amount){
    if (!array_of_arrays || !array_lenghts){
        return NULL;
    }
    int** copy_array_2 = (int**) malloc (array_amount * sizeof(int*));
    if (!copy_array_2){
        return NULL;
    }
    for (int i = 0; i < array_amount; i++){
        copy_array_2[i] = copy_array(array_of_arrays[i], array_lenghts[i]);
    }
    return copy_array_2;
}

/*
 * Libera toda la memoria asociada a un arreglo de arreglos.
 *
 * array_of_arrays: un arreglo de punteros a arreglos de enteros
 * array_lenghts: un arreglo con los largos de cada arreglo en array_of_arrays
 * array_amount: la cantidad de arreglos
 */
void free_array_of_arrays(int **array_of_arrays, int *array_lenghts, int array_amount){
    if (!array_of_arrays || !array_lenghts){
        return;
    }
    for (size_t f = 0; f < array_amount; f++){
        free(array_of_arrays[f]);  
    }
    free(array_of_arrays);
    free(array_lenghts);
    return;
}
