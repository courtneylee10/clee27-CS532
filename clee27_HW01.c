#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int sumOfDigits(int n)
{
    int sum = 0;
    
    if (n <= 0)
    {
        return -1;
    }

    while (n > 0)
    {
        sum = sum + (n % 10);
        n = n / 10;
    }
    return sum;
}


int UABMaxMinDiff(int arr[], int size)
{
    int max = arr[0];
    int min = arr[0];

    for (int i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return max - min;
}


int *replaceEvenWithZero(int arr[], int size)
{
    int *newArray = malloc(size * sizeof(int));

    if (newArray == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 0)
        {
            newArray[i] = 0;
        }
        else
        {
            newArray[i] = arr[i];
        }
    }

    return newArray;
}


int perfectSquare(int n)
{
    if (n < 0)
    {
        return 0;
    }

    for (int k = 0; k * k <= n; k++)
    {
        if (k * k == n)
        {
            return 1;
        }
    }
    
    return 0;
}


int countVowels(char s[])
{
    int count = 0;
    
    for (int i = 0; s[i] != '\0'; i++)
    {
        char ch = tolower((unsigned char)s[i]);

        if (ch == 'a' ||
            ch == 'e' ||
            ch == 'i' ||
            ch == 'o' ||
            ch == 'u')
        {
            count++;
        }
    }
    return count;
}


int main(void)
{
    printf("sumOfDigits(123) = %d\n", sumOfDigits(123));
    printf("sumOfDigits(405) = %d\n", sumOfDigits(405));
    printf("sumOfDigits(0) = %d\n", sumOfDigits(0));
    printf("sumOfDigits(7) = %d\n", sumOfDigits(7));
    printf("sumOfDigits(-308) = %d\n", sumOfDigits(-308));

    printf("\n");


    int arr1[] = {3, 7, 2, 9};
    int arr2[] = {5, 5, 5, 5, 5, 5};
    int arr3[] = {-2, 4, -1, 6, 5};

    printf("UABMaxMinDiff(arr1) = %d\n",
           UABMaxMinDiff(arr1, 4));

    printf("UABMaxMinDiff(arr2) = %d\n",
           UABMaxMinDiff(arr2, 6));

    printf("UABMaxMinDiff(arr3) = %d\n",
           UABMaxMinDiff(arr3, 5));

    printf("\n");


    int arr4[] = {1, 2, 3, 4};
    int arr5[] = {2, 4, 6};
    int arr6[] = {1, 3, 5};

    int *result1 = replaceEvenWithZero(arr4, 4);
    int *result2 = replaceEvenWithZero(arr5, 3);
    int *result3 = replaceEvenWithZero(arr6, 3);

    printf("replaceEvenWithZero(arr4) = [");
    for (int i = 0; i < 4; i++)
    {
        printf("%d", result1[i]);

        if (i < 3)
        {
            printf(", ");
        }
    }
    printf("]\n");

    printf("replaceEvenWithZero(arr5) = [");
    for (int i = 0; i < 3; i++)
    {
        printf("%d", result2[i]);

        if (i < 2)
        {
            printf(", ");
        }
    }
    printf("]\n");

    printf("replaceEvenWithZero(arr6) = [");
    for (int i = 0; i < 3; i++)
    {
        printf("%d", result3[i]);

        if (i < 2)
        {
            printf(", ");
        }
    }
    printf("]\n");

    free(result1);
    free(result2);
    free(result3);

    printf("\n");

     printf("perfectSquare(16) = %s\n",
           perfectSquare(16) ? "True" : "False");

    printf("perfectSquare(15) = %s\n",
           perfectSquare(15) ? "True" : "False");

    printf("perfectSquare(25) = %s\n",
           perfectSquare(25) ? "True" : "False");

    printf("perfectSquare(36) = %s\n",
           perfectSquare(36) ? "True" : "False");

    printf("\n");


     printf("countVowels(\"Hello World\") = %d\n",
           countVowels("Hello World"));

    printf("countVowels(\"UAB CS\") = %d\n",
           countVowels("UAB CS"));

    printf("countVowels(\"Python\") = %d\n",
           countVowels("Python"));

    printf("countVowels(\"aeiou\") = %d\n",
           countVowels("aeiou"));

    return 0;
}

