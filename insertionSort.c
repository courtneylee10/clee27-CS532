/*
Compile the program
gcc -o insertionSort insertionSort.c

Run the program
./insertionSort 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100


void readStrings(char **strings, int n);
void insertionSort(char **strings, int n);
void displayStrings(char **strings, int n);

int main(void)
{
    int n;


    printf("Enter the number of strings: ");
    scanf("%d", &n);

   
    char **strings = malloc(n * sizeof(char *));

    if (strings == NULL)
    {
        printf("Failed.\n");
        return 1;
    }

    
    readStrings(strings, n);

  
    insertionSort(strings, n);


    printf("\nSorted strings:\n");
    displayStrings(strings, n);

   
    for (int i = 0; i < n; i++)
    {
        free(strings[i]);
    }

 
    free(strings);

    return 0;
}



void readStrings(char **strings, int n)
{
    char temporary[MAX_LENGTH];

    for (int i = 0; i < n; i++)
    {
        printf("Enter string %d: ", i + 1);

        scanf("%999s", temporary);

        
        int length = strlen(temporary);

     
        strings[i] = malloc((length + 1) * sizeof(char));

        if (strings[i] == NULL)
        {
            printf("Failed.\n");
            exit(1);
        }

       
        strcpy(strings[i], temporary);
    }
}



void insertionSort(char **strings, int n)
{
    for (int i = 1; i < n; i++)
    {
        char *key = strings[i];
        int j = i - 1;

  
        while (j >= 0 && strcmp(strings[j], key) > 0)
        {
            strings[j + 1] = strings[j];
            j--;
        }

        strings[j + 1] = key;
    }
}



void displayStrings(char **strings, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", strings[i]);
    }
}