/*
Compile the program:
gcc -o prime prime.c

Run the program:
./prime
*/

#include <stdio.h>

int main(int argc, char** argv) {
    int given_number;
    int is_prime = 1;

    printf("Enter an integer: ");
    scanf("%d" , &given_number);

    if (given_number <= 1) {
        is_prime = 0;
    } else {
        for (int i = 2; i <= given_number / 2; i++) {
            if (given_number % i == 0) {
                is_prime = 0;
                break;
            }
        }
    }
    if (is_prime) {
        printf("The number is prime\n");
    } else {
        printf("The number is not prime\n");
    }

    return 0;
}