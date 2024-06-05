#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sysexits.h>
#define MAX_LINE 80 /* The maximum length command */

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
    char *history[MAX_LINE * 10];
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
        char *args[numTokens + 1];
        // Print the words
        for (int i = 0; i < numTokens; i++)
        {
            printf("Word %d: %s\n", i + 1, words[i]);
            history[record + i] = words[i];
            args[i] = words[i];
        }
        // history saving
        int j = 0;
        record = record + numTokens;
        do
        {
            printf("%s:\n", history[j]);
            j++;
        } while (history[j] != NULL);
        // exit maybe later i will do it
    
        if (fork() == 0)
        { // child
            char *command = args[0];
            args[numTokens + 1] = NULL;
            printf("Executing the command\n");
            int status_code = execvp(command, args);

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

            if (strcmp(args[numTokens - 1], "&") != 10)
            {

                wait(NULL);
            }
        }
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
    }
    return 0;
}
