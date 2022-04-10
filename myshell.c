#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/*
 * myshell.c
 *
 *  Created on: Apr 10, 2022
 */

char remove_newline(char *str)
{
    char *pos;
    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';
    return *str;
}

void run(char *command[])
{
    printf("Executing command: %s", command[0]);
    int return_value = system(command[0]);

    if (return_value == 0)
    {
        printf("Processo X finalizou normalmente com status %i\n\n", return_value);
    }
    else
        printf("Processo de numero X falhou com status %d\n\n", return_value);

    // int comparisson = strcmp(command, "run\n");
}
/**
 * readline - read a line of input from stdin
 */
int main()
{

    while (1)
    {
        char palavras[4096];
        int param_index = 0;

        char *token;
        char *command = NULL;
        char *params;
        char *operation;
        printf("myshell>");
        fgets(palavras, 4096, stdin);

        /* get the first token */
        token = strtok(palavras, " ");
        operation = token;

        int argvLen = strlen(palavras);
        command = malloc(argvLen + 1);

        while (token != NULL)
        {
            if (param_index != 0)
            {
                strcat(command, token);
                strcat(command, " ");
            }
            token = strtok(NULL, " ");
            param_index = 1;
        }

        *operation = remove_newline(operation);
        printf("operation : %s\n", operation);

        if (strcmp(operation, "start") == 0)
        {
            printf("start\n");
        }
        else if (strcmp(operation, "exit") == 0 || strcmp(operation, "quit") == 0)
        {
            exit(0);
        }
        else if (strcmp(operation, "run") == 0)
        {
            run(&command);
        }
        else if (strcmp(operation, "stop") == 0)
        {
            printf("stop\n");
        }
        else if (strcmp(operation, "continue") == 0)
        {
            printf("continue\n");
        }
        else
        {
            printf("Invalid operation\n");
        }
    }
    return 0;
}
