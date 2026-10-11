
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#define MAX_LINE 1024
#define MAX_ARGS 100

void get_time_string(time_t current_time, char *buffer,
                     size_t buffer_size)
{
    struct tm *time_info = localtime(&current_time);

    if (time_info == NULL) {
        snprintf(buffer, buffer_size, "Unknown time");
        return;
    }

    strftime(buffer, buffer_size, "%a %b %d %H:%M:%S %Y",
             time_info);
}

int main(int argc, char *argv[])
{
    FILE *input_file;
    FILE *log_file;
    char line[MAX_LINE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    input_file = fopen(argv[1], "r");

    if (input_file == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    log_file = fopen("output.log", "w");

    if (log_file == NULL) {
        perror("Error opening output.log");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), input_file) != NULL) {
        char *args[MAX_ARGS];
        int arg_count = 0;
        char *token;
        pid_t pid;
        int status;
        struct timeval start_time, end_time;
        char start_string[100], end_string[100];

        line[strcspn(line, "\n")] = '\0';

        token = strtok(line, " \t");

        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }

        args[arg_count] = NULL;

        if (arg_count == 0) {
            continue;
        }

        gettimeofday(&start_time, NULL);
        get_time_string(start_time.tv_sec, start_string,
                        sizeof(start_string));

        pid = fork();

        if (pid < 0) {
            perror("fork");
            fprintf(log_file, "%s\t%s\tERROR: fork failed\n",
                    line, start_string);
            continue;
        }

        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            _exit(127);
        }

        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            continue;
        }

        gettimeofday(&end_time, NULL);
        get_time_string(end_time.tv_sec, end_string,
                        sizeof(end_string));

        fprintf(log_file, "%s\t%s\t%s\n",
                line, start_string, end_string);
        fflush(log_file);
    }

    fclose(input_file);
    fclose(log_file);

    printf("Commands completed. Results saved to output.log\n");

    return EXIT_SUCCESS;
}
