/*
Compile the program:
gcc -o insertionSort insertionSort.c

Run the program:
./insertionSort
*/

#include <stdio.h>

int main() {
    int N;

    printf("Enter the number of array elements: ");
    scanf("%d", &N);

    int array[N];

    printf("Enter %d elements:\n", N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &array[i]);
    }

    for (int i = 1; i < N; i++) {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }

    printf("Sorted array: ");

    for (int i = 0; i < N; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");

    return 0;
}