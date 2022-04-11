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
    int child_pid;
    if (pid > 0)
    {
        child_pid = pid;
        printf("child pid : %d\n", pid);
    }
    int status;

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
        break;
    default:
        waitpid(pid, &status, WUNTRACED);
        printf("Processo %i terminado com status : %i\n", child_pid, status);
        break;
    }
    return 0;
}

int start(char *command[], char *params[])
{
    int pid = fork();
    int child_pid;
    if (pid > 0)
    {
        child_pid = pid;
        printf("child pid : %d\n", pid);
    }
    int status;

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
        break;
    default:
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
        int status;

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
                param_index = 1;
                break;
            case 1:
                command = token;
                param_index = 2;
                break;

            default:
                strcat(params, token);
                strcat(params, " ");
                break;
            }

            token = strtok(NULL, " ");
        }

        if (strcmp(operation, "\n") == 0)
        {
            free(params);
            continue;
        }

        *operation = remove_newline(operation);
        *command = remove_newline(command);
        *params = remove_newline(params);

        if (strcmp(operation, "start") == 0)
        {
            start(&command, &params);
        }
        else if (strcmp(operation, "exit") == 0 || strcmp(operation, "quit") == 0)
        {
            free(params);
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
        else if (strcmp(operation, "kill") == 0)
        {
            printf("%d\n", getpid());
            printf("kill\n");
            if (kill(atoi(command), SIGKILL) < 0)
                printf("Erro ao finalizar processo %i\n", atoi(command));
            else
                printf("Processo %i finalizado com status %i\n", atoi(command), status);
        }
        else
        {
            printf("Invalid operation\n");
        }
        free(params);
    }
    return 0;
}
