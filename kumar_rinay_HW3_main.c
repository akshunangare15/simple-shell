/**************************************************************
* Class:  CSC-415-01 Fall 2020
* Name: Rinay Kumar
* Student ID: 913859133
* GitHub ID: rinaykumar
* Project: Assignment 3 – Simple Shell
*
* File: kumar_rinay_HW3_main.c
*
* Description:
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 512
#define ARGS_SIZE 5
#define LOOP 1

int parseInput(char *inputLine, char **arguments) {
    int count = 0;
    char *delimiter = " \n\t";

    arguments[count] = strtok(inputLine, delimiter);

    while ((arguments[count] != NULL) && (count+1 < ARGS_SIZE)) {
        arguments[++count] = strtok((char *) 0, delimiter);
    }

    return count;
}

int main(int argc, char * argv[]) {
    // char inputLine[BUFFER_SIZE];
    char *arguments[ARGS_SIZE];
    char *inputLine;
    // char *arguments;
    int argumentCount;
    int waitStatus;

    inputLine = malloc(BUFFER_SIZE);
    // arguments = malloc(ARGS_SIZE);

    while (LOOP) {
        printf("%s", argv[1]);

        fgets(inputLine, BUFFER_SIZE, stdin);

        if (feof(stdin)) {
            printf("\n");
            break;
            exit(EXIT_FAILURE);
        }

        if (ferror(stdin)) {
            printf("ERROR: Could not read input\n");
            break;
            exit(EXIT_FAILURE);
        }

        argumentCount = parseInput(inputLine, arguments);

        if (argumentCount == 0) {
            printf("ERROR: Empty command line\n");
            continue;
        }

        if (strcmp(arguments[0], "exit") == 0) {
            exit(EXIT_SUCCESS);
        }

        pid_t ret;
        int execRet;

        switch((ret = fork())) {
            case -1: 
                printf("ERROR: Fork failed\n");
                exit(EXIT_FAILURE);

            case 0: // Child              
                execRet = execvp(arguments[0], arguments);
                if (execRet == -1) {
                    printf("ERROR: Invalid command\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default: // Parent 
                if (wait(&waitStatus) == -1) {
                    printf("ERROR: Parent error\n");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Child %d, exited with %d\n", ret, WEXITSTATUS(waitStatus));
                }
                break;
        }
    }

    return 0;
}
