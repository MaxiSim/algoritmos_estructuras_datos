#include <stdlib.h>
#include <stdio.h>

// 1.1 Ejercicios para entrar en calor
int prime (int n){
    int i;
    for (i=2; i<n; i++){
        if (n%i==0){
            return 0;
        }
    }
    return 1;
}

int sum_odd (int n){
    int sum = 0;
    for (int i = 1; i <= n; i+=2){
        sum += i;
    }
    return sum;
}

// 1.2 Punteros y arreglos

void myFunc (int* a, int b){
    (*a)++;
    b++;
}

// main 1

// int main (void){
//     prime(5);
//     sum_odd(10);
//     int a = 10;
//     int b = 10;
//     myFunc(&a, b);
//     printf("a = %d, b = %d\n", a, b);


//     return 0;
// }

// main 2

int main (void){
    int x = 10;
    int* px = &x;

    printf ("%d \n", px);
    printf ("%d \n", (*px));
    
    (*px)++;

    printf ("%d \n", px);
    printf ("%d \n", (*px));

    return 0;
}