#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

int start(char *params[], bool wait)
{
    int pid = fork();
    int child_pid;
    if (pid > 0)
    {
        child_pid = pid;
        printf("Pid do novo processo inicializado : %d\n", pid);
    }
    int status;

    switch (pid)
    {
    case -1:
        perror("fork error");
        break;

    case 0:
        if (execvp(params[0], params) == -1)
        {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        if (wait)
        {
            waitpid(pid, &status, WUNTRACED);
            printf("Processo %i finalizou com status : %i\n", child_pid, status);
        }

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
        char *argv[4096];
        char *operation;
        printf("myshell>");
        fgets(palavras, 4096, stdin);

        token = strtok(palavras, " ");
        operation = token;

        int argvLen = strlen(palavras);

        while (token != NULL)
        {
            switch (param_index)

            {
            case 0:
                operation = token;
                break;

            default:
                *token = remove_newline(token);
                argv[param_index - 1] = token;
                break;
            }
            param_index++;
            token = strtok(NULL, " ");
        }

        if (argv[param_index] == "")
            argv[param_index] = '\0';
        argv[param_index] = NULL;
        int i = 0;

        if (strcmp(operation, "\n") == 0)
        {
            continue;
        }

        *operation = remove_newline(operation);

        if (strcmp(operation, "start") == 0)
        {
            start(argv, false);
        }
        else if (strcmp(operation, "exit") == 0 || strcmp(operation, "quit") == 0)
        {
            exit(0);
        }
        else if (strcmp(operation, "run") == 0)
        {
            start(argv, true);
        }
        else if (strcmp(operation, "wait") == 0)
        {
            int status;
            int response = wait(&status);
            if (response == -1)
            {
                printf("nao ha processos restantes\n");
            }
            if (response >= 0)
            {
                if (WIFEXITED(status))
                {
                    const int es = WEXITSTATUS(status);
                    printf("Processo %i foi finalizado com status %d\n", response, status);
                }
            }
        }
        else if (strcmp(operation, "stop") == 0)
        {
            if (kill(atoi(argv[0]), SIGSTOP) == -1)
            {
                printf("Processo não encontrado\n");
            }
            else
            {
                printf("Processo %i parado\n", atoi(argv[0]));
            }
        }
        else if (strcmp(operation, "continue") == 0)
        {
            if (kill(atoi(argv[0]), SIGCONT) == -1)
            {
                printf("Processo não encontrado\n");
            }
            else
            {
                printf("Processo %i continuado\n", atoi(argv[0]));
            }
        }
        else if (strcmp(operation, "kill") == 0)
        {
            if (kill(atoi(argv[0]), SIGKILL) < 0)
                printf("Erro ao finalizar processo %i\n", atoi(argv[0]));
            else
                printf("Processo %i finalizado com status %i\n", atoi(argv[0]), status);
        }

        else
        {
            printf("Comando inválido\n");
        }
    }
    return 0;
}
