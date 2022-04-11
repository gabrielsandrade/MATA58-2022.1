#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

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

int run(char *command[], char *params[])
{
    int pid = fork();
    int *status;

    switch (pid)
    {
    case -1:
        perror("fork error");
        break;

    case 0:
        if (execvp(command[0], params) == -1)
        {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
        printf("Processo terminado com status : %d\n", *status);
        break;
    default:
        waitpid(pid, status, WUNTRACED);
        break;
    }
    return 0;
}

int main()
{

    while (1)
    {
        char palavras[4096];
        int param_index = 0;
        long status;

        char *token;
        char *command = NULL;
        char *params = NULL;
        char *operation;
        printf("myshell>");
        fgets(palavras, 4096, stdin);

        /* get the first token */
        token = strtok(palavras, " ");
        operation = token;

        int argvLen = strlen(palavras);
        params = malloc(argvLen + 1);

        while (token != NULL)
        {
            switch (param_index)

            {
            case 0:
                operation = token;
                break;
            case 1:
                command = token;
                break;

            default:
                strcat(params, token);
                strcat(params, " ");
                break;
            }

            token = strtok(NULL, " ");
            param_index += 1;
        }

        *operation = remove_newline(operation);
        *command = remove_newline(command);
        *params = remove_newline(params);
        printf("operation : %s\n", operation);
        printf("command : %s\n", command);
        printf("params : %s\n", params);

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
            run(&command, &params);
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
        free(params);
    }
    return 0;
}
