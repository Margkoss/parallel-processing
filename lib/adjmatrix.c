#include <stdio.h>
#include <stdlib.h>

#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define CLEAR "\033[0m"

int **generateAdjMatrix(char **matrixLines, int *len)
{
    char *firstLine = malloc(strlen(matrixLines[0]) + 1);
    strcpy(firstLine, matrixLines[0]);

    // Tokenize the string to get the first number
    char *sizeString = strtok(firstLine, " ");
    strtok(NULL, " ");
    char *edgesString = strtok(NULL, " ");
    int size;
    int edges;

    // Parse strings to int
    sscanf(sizeString, "%d", &size);
    sscanf(edgesString, "%d", &edges);

    // Create 2d empty 2d array from size
    int **arr = calloc(size, sizeof(int *));
    // Throw error in chance of failure to create array in memory
    if (!arr)
    {
        fprintf(stderr, "%sExitting...\nNot enough memory\n", RED);
        exit(1);
    }

    for (int i = 0; i < size; i++)
    {
        arr[i] = calloc(size, sizeof(int));
        // Throw error in chance of failure to create array in memory
        if (!arr[i])
        {
            fprintf(stderr, "%sExitting...\nNot enough memory\n", RED);
            exit(1);
        }
    }

    // Create adjacency matrix from data
    for (int i = 1; i < edges; i++)
    {
        // Get the two numbers
        int first;
        int second;
        sscanf(strtok(matrixLines[i], " "), "%d", &first);
        sscanf(strtok(NULL, " "), "%d", &second);

        arr[first - 1][second - 1] = 1;
    }

    *len = size;
    return arr;
}
