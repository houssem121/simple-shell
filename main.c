#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 80 /* The maximum length command */

char **parseInput(char input[], int *numWords)
{
    const char *separators = " ";
    char *strToken = strtok(input, separators);
    char **wordArray = NULL;
    int count = 0;
    while (strToken != NULL)
    {
        wordArray = (char **)realloc(wordArray, (count + 1) * sizeof(char *));//size of (pointer to char )
        wordArray[count] = strdup(strToken); // Copy the token and then gives the pointer to be stored in the array of pointer to pointer to char **
        count++;//inc number of tokens 
        printf("%s\n", strToken);
        // On demande le token suivant.
        strToken = strtok(NULL, separators);
    }
    *numWords = count;
    return wordArray;
};

int main(int argc, char *argv[])
{

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

        // Print the words
        for (int i = 0; i < numTokens; i++)
        {   
            printf("Word %d: %s\n", i + 1, words[i]);
            free(words[i]); // Free memory for each word
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
