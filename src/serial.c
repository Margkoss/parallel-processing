#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define CLEAR "\033[0m"

// User libs
#include "../lib/loadfile.c"
#include "../lib/adjmatrix.c"
#include "../lib/queue.c"

int main(int argc, char *argv[])
{
    printf("%sPART ONE: SERIAL EXECUTION%s\n", BLUE, CLEAR);
    printf("Reading file ...\n");

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

    printf("Generating adjacency matrix for input ...\n");

    // Create an adjacency matrix to represent the DAG
    int size;
    int **dag = generateAdjMatrix(words, &size);

    // Words array isn't needed so free memory
    for (int i = 0; i < length; i++)
        free(words[i]);
    free(words);

    printf("Checking that the graph is directed and acyclic ...\n");

    // Check that there is nothing on the main diagonal because that would mean self-loops
    for (int i = 0; i < size; i++)
    {
        if (dag[i][i])
        {
            fprintf(stderr, "%sExitting...\nThe graph given is not a DAG because of self loop in (%d, %d)\n", RED, ++i, i);
            exit(1);
        }
    }

    // Check that there is no symmetry that would mean loops
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            if (i == j)
                continue;

            if (dag[i][j] == 1 && dag[j][i] == 1)
            {
                fprintf(stderr, "%sExitting...\nThe graph given is not a DAG because cyclic dependency [%d, %d]\n", RED, ++i, ++j);
                exit(1);
            }
        }

    printf("Kahn's algorithm serial execution...\n");

    /*
    *   Begin Kahn's algorithm
    */

    clock_t t;
    t = clock();
    // Place all nodes with no incoming edge on to a queue
    node_t *toCheck = create();
    node_t *sorted = create();
    // Incoming edges are represented by ones in columns (ref -> https://en.wikipedia.org/wiki/Adjacency_matrix#Directed_graphs)
    for (int j = 0; j < size; j++)
    {
        int foundOne = 0;
        for (int i = 0; i < size; i++)
        {
            if (dag[i][j])
            {
                foundOne++;
                break;
            }
        }
        if (!foundOne)
            enqueue(j, &toCheck);
    }

    // Dequeue elements and look if they have incoming edges
    while (!isEmpty(toCheck))
    {
        int vertex = dequeue(&toCheck);
        enqueue(vertex, &sorted);
        node_t *neigboors = create();
        for (int i = 0; i < size; i++)
        {
            if (dag[vertex][i])
            {
                enqueue(i, &neigboors);
                dag[vertex][i] = 0;
            }
        }
        while (!isEmpty(neigboors))
        {
            int single = dequeue(&neigboors);
            int foundOne = 0;
            for (int i = 0; i < size; i++)
            {
                if (dag[i][single])
                {
                    foundOne++;
                    break;
                }
            }
            if (!foundOne)
                enqueue(single, &toCheck);
        }
    }
    t = clock() - t;
    double timeTaken = ((double)t) / CLOCKS_PER_SEC;

    printf("Serial execution took %f seconds to finish.\n", timeTaken);
    return 0;
}
