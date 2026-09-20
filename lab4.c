#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *file1;
    FILE *file2;
    char buffer[1024];
    size_t bytesRead;


    if (argc != 3)
    {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }


    if (strcmp(argv[1], argv[2]) == 0)
    {
        printf("Error: The two filenames cannot be the same.\n");
        return 1;
    }


    file1 = fopen(argv[1], "ab");

    if (file1 == NULL)
    {
        printf("Error: Could not open %s.\n", argv[1]);
        return 1;
    }


    file2 = fopen(argv[2], "rb");

    if (file2 == NULL)
    {
        printf("Error: Could not open %s.\n", argv[2]);
        fclose(file1);
        return 1;
    }


    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file2)) > 0)
    {
        if (fwrite(buffer, 1, bytesRead, file1) != bytesRead)
        {
            printf("Error: Unable to write to %s.\n", argv[1]);
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }


    if (ferror(file2))
    {
        printf("Error: Unable to read from %s.\n", argv[2]);
        fclose(file1);
        fclose(file2);
        return 1;
    }


    fclose(file1);
    fclose(file2);

    printf("Files successfully concatenated.\n");

    return 0;
}