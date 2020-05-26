#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User libs
#include "stack.h"

const char *RED = "\033[0;31m";
const int STEP_SIZE = 100;

char **loadFile(char *filename, int *len);
int **generateAdjMatrix(char **matrixLines, int len);

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        fprintf(stderr, "%sExitting...\nNo filename was given\n", RED);
        exit(1);
    }

    //Reading the file
    //Define a length for the file to keep track
    int length = 0;
    //Load file into memorry
    char **words = loadFile(argv[1], &length);

    // Create an adjacency matrix to represent the DAG
    int **dag = generateAdjMatrix(words, length);

    // Words array isn't needed so free memory
    for (int i = 0; i < length; i++)
        free(words[i]);
    free(words);

    return 0;
}

int **generateAdjMatrix(char **matrixLines, int len)
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
    int **arr = malloc(size * sizeof(int *));
    // Throw error in chance of failure to create array in memory
    if (!arr)
    {
        fprintf(stderr, "%sExitting...\nNot enough memory\n", RED);
        exit(1);
    }

    for (int i = 0; i < size; i++)
    {
        arr[i] = malloc(size * sizeof(int));
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

    return arr;
}

char **loadFile(char *filename, int *len)
{
    // Open file for reading
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "%sExitting...\nCould not open %s for reading contents\n", RED, filename);
        exit(1);
    }

    int arrLength = STEP_SIZE;
    //Allocate space for arrLength char*
    char **lines = (char **)malloc(arrLength * sizeof(char *));

    //Read from file into a 200 char buffer
    char buf[200];
    int i = 0;
    while (fgets(buf, 200, f))
    {
        //Ignore all lines begining with %
        if (buf[0] == '%')
        {
            continue;
        }
        //Check if the lines array is full, and extend as needed
        if (i >= arrLength)
        {
            arrLength += STEP_SIZE;
            char **newlines = realloc(lines, arrLength * sizeof(char *));
            if (!newlines)
            {
                fprintf(stderr, "%sExitting...\nCould not read entire file\n", RED);
                exit(1);
            }
            lines = newlines;
        }

        //Trim off new line char at end of line
        buf[strlen(buf) - 1] = '\0';

        //Get the length of the buffer and allocate memory for the line
        char *str = (char *)malloc((strlen(buf) + 1));
        strcpy(str, buf);
        lines[i] = str;
        i++;
    }

    *len = i;
    return lines;
}
