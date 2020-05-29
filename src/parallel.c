#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define CLEAR "\033[0m"

// User libs
#include "../lib/loadfile.c"
#include "../lib/adjmatrix.c"
#include "../lib/queue.c"

int main(int argc, char *argv[])
{
    printf("%sPART TWO: PARALLEL EXECUTION%s\n", BLUE, CLEAR);
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

    printf("Kahn's algorithm parallel execution...\n");

    /*
    *   Begin Kahn's algorithm
    */

    double t = omp_get_wtime();
    // Place all nodes with no incoming edge on to a queue utilizing available threads
    node_t *toCheck = create();
    node_t *sorted = create();
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nThreads = omp_get_num_threads();

        for (int y = id; y < size; y += nThreads)
        {
            int foundOne = 0;
            for (int x = 0; x < size; x++)
            {
                if (dag[x][y])
                {
                    foundOne++;
                    break;
                }
            }
            if (!foundOne)
            {
#pragma omp critical
                {
                    enqueue(y, &toCheck);
                }
            }
        }
        // Add barrier so that the check queue is so no vertex with no incoming edges is left behind
#pragma omp barrier
        // Each thread is going to remove edges from one vertice from each iteration and check
        // if this creates verices of in degree 0 to neigboors
        // https://en.wikipedia.org/wiki/Topological_sorting#Parallel_algorithms
        while (!isEmpty(toCheck))
        {
            int vertex;
#pragma omp critical
            {
                vertex = dequeue(&toCheck);
                if (vertex == -1)
                    exit(0);
                enqueue(vertex, &sorted);
            }
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
                {
#pragma omp critical
                    {
                        enqueue(single, &toCheck);
                    }
                }
            }
        }
    }
    printf("Parallel execution took %f seconds to finish.\n", omp_get_wtime() - t);
    return 0;
}
