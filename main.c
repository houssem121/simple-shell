#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#define MAX_LINE 80 /* The maximum length command */
enum SpecialCommands
{
    CMD_HISTORY = 1, // Example: "!!"

};
char **parseInput(char input[], int *numWords)
{
    const char *separators = " ";
    char *strToken = strtok(input, separators);
    char **wordArray = NULL;
    int count = 0;
    while (strToken != NULL)
    {
        wordArray = (char **)realloc(wordArray, (count + 1) * sizeof(char *)); // size of (pointer to char )
        wordArray[count] = strdup(strToken);                                   // Copy the token and then gives the pointer to be stored in the array of pointer to pointer to char **
        count++;                                                               // inc number of tokens
        // printf("%s\n", strToken);
        //  On demande le token suivant.
        strToken = strtok(NULL, separators);
    }
    *numWords = count;

    return wordArray;
};

int main(void)
{
    char *history[MAX_LINE];
    int record = 0;
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run)
    {
        int numTokens;
        char input[MAX_LINE];

        printf("osh>");
        fflush(stdout); // Ensure "osh>" is displayed immediately
        if (fgets(input, MAX_LINE, stdin) == NULL)
        {
            perror("Error reading input");
        }

        char **words = parseInput(input, &numTokens);
        printf("%d", numTokens);
        char *args[numTokens - 2];
        // Print the words
        for (int i = 0; i < numTokens; i++)
        {
            printf("Word %d: %s\n", i + 1, words[i]);
            args[i] = words[i];
        }

        record = record + numTokens;
        char *argss[numTokens];
        if (args[0] != NULL && strcmp(args[0], "exit") != 10)
        {
            if (strcmp(args[0], "!!") == 10)
            { // history part only works on simpke command i didnt make those for < > or  |

                if (history[0] == NULL)
                {
                    printf("there is no history cmd \n");
                }

                else if (fork() == 0)
                { // child
                    char *command = history[0];

                    history[numTokens + 1] = NULL;
                    printf("history:%s", history[1]);
                    for (int j = 0; j < numTokens; j++)
                    {
                        printf("hispojfedo:%s\n", history[j]);
                    }
                    if (command == NULL)
                    {
                        printf("there is no history cmd");
                        break;
                    }
                    printf("Executing the command\n");
                    int status_code = execvp(command, history);

                    if (status_code == -1)
                    {
                        printf("Process did not terminate correctly\n");
                        exit(1);
                    }

                    else
                    {
                        printf("This line will not be printed if execvp() runs correctly\n");
                    }
                } //
                else
                { // parent

                    if (strcmp(history[numTokens - 1], "&") != 10)
                    {

                        wait(NULL);
                    }
                }
            }
            else
            {

                if (fork() == 0)
                { // child
                    char *command = args[0];
                    args[numTokens] = NULL;
                    printf("Executing the command\n");

                    if (strcmp(args[numTokens - 2], ">") == 0 || strcmp(args[numTokens - 2], "<") == 0 || strcmp(args[numTokens - 2], "|") == 0)
                    {

                        for (int j = 0; j < numTokens - 2; j++)
                        {
                            argss[j] = args[j];
                        }
                        argss[numTokens - 2] = NULL;
                        if (strcmp(args[numTokens - 2], "<") == 0) // redirect input
                        {
                            int fd = open(args[numTokens - 1], O_RDONLY);
                            if (fd < 0)
                            {
                                perror("open");
                                return 1;
                            }
                            if (dup2(fd, STDIN_FILENO) < 0)
                            {
                                perror("dup2");
                                close(fd);
                                exit(1);
                            }
                            close(fd);

                            int status_code = execvp(command, argss);

                            // If execvp() fails
                            if (status_code < 0)
                            {
                                perror("execvp");
                                exit(1);
                            }
                        }
                        else if (strcmp(args[numTokens - 2], "|") == 0)
                        {
                            pid_t pid;
                            int fd[2];
                            printf("hey i am  | \n");
                            pipe(fd);
                            pid = fork();

                            if (pid == 0)
                            {
                                printf("hey i am  | : %s %s\n", argss[0], argss[1]);
                                close(fd[0]);               // Close unused read end
                                dup2(fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
                                close(fd[1]);               // Close the write end after duplicating

                                int status_code = execvp(argss[0], argss);

                                // If execvp() fails
                                if (status_code < 0)
                                {
                                    perror("execvp");
                                    exit(1);
                                }
                                exit(1);
                            }
                            else
                            {
                                char *argss[2];
                                argss[0] = args[numTokens - 1];
                                argss[1] = NULL;
                                printf("hey i am  | : %s %s\n", argss[0], argss[1]);
                                close(fd[1]);              // Close unused write end
                                dup2(fd[0], STDIN_FILENO); // Redirect stdin to the pipe
                                close(fd[0]);              // Close the read end after duplicating

                                int status_code = execvp(args[0], argss);

                                // If execvp() fails
                                if (status_code < 0)
                                {
                                    perror("execvp");
                                    exit(1);
                                }
                                exit(1);
                            }

                            int status;
                            close(fd[1]);
                            close(fd[0]);
                            waitpid(pid, &status, 0);
                        }
                        else
                        { // redirect output
                            int fd = open(args[numTokens - 1], O_CREAT | O_RDWR, 0666);
                            if (fd < 0)
                            {
                                printf("Couldn't open file: %d\n", errno);
                            }
                            if (dup2(fd, STDOUT_FILENO) < 0)
                            {
                                printf("Couldn't redirect stdout: %d\n", errno);
                                perror("dup2");
                                close(fd);
                            }
                            close(fd);

                            int status_code = execvp(command, argss);

                            if (status_code == -1)
                            {
                                printf("Process did not terminate correctly\n");
                                exit(1);
                            }
                        }
                    }
                    else
                    {

                        int status_code = execvp(command, args);

                        if (status_code == -1)
                        {
                            printf("Process did not terminate correctly\n");
                            exit(1);
                        }
                    }
                } //
                else
                { // parent

                    if (strcmp(args[numTokens - 1], "&") != 10)
                    {
                        for (int j = 0; j < numTokens; j++)
                        {

                            history[j] = args[j];

                            printf("%s", history[j]);
                        }
                        wait(NULL);
                    }
                }
            }
        }
        else
        {

            exit(0);
        }
        
    }
    return 0;
}
