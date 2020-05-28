#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User libs

// Linked list definition
typedef struct node
{
    int data;
    struct node *next;
} node_t;

const char *RED = "\033[0;31m";
const int STEP_SIZE = 100;

char **loadFile(char *filename, int *len);
int **generateAdjMatrix(char **matrixLines, int *len);

// Linked list queue functions
node_t *create();
void enqueue(int data, node_t **head);
void print_list(node_t *head);
int dequeue(node_t **head);
int isEmpty(node_t *head);

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
    int size;
    int **dag = generateAdjMatrix(words, &size);

    // Words array isn't needed so free memory
    for (int i = 0; i < length; i++)
        free(words[i]);
    free(words);

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

    /*
    *   Begin Kahn's algorithm
    */

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
    print_list(sorted);

    return 0;
}

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

// empty linked list queue
node_t *create()
{
    return NULL;
}

void enqueue(int data, node_t **head)
{
    if (!(*head))
    {
        (*head) = (struct node *)malloc(1 * sizeof(struct node));
        (*head)->next = NULL;
        (*head)->data = data;
    }
    else
    {
        node_t *current = (*head);
        while (current->next != NULL)
        {
            current = current->next;
        }

        /* now we can add a new variable */
        current->next = (node_t *)malloc(sizeof(node_t));
        current->next->data = data;
        current->next->next = NULL;
    }
}

// Debugging function that traverses linked list and prints elements
void print_list(node_t *head)
{
    if (!head)
    {
        printf("No items yet\n");
    }
    else
    {

        node_t *current = head;

        while (current != NULL)
        {
            printf("%d \n", current->data);
            current = current->next;
        }
        printf("\n");
    }
}

int dequeue(node_t **head)
{
    if (!(*head))
    {
        return -1;
    }
    else
    {
        node_t *temp;
        int toPop = (*head)->data;

        temp = (*head);
        (*head) = (*head)->next;
        temp->next = NULL;

        free(temp);
        return toPop;
    }
}

int isEmpty(node_t *head)
{
    return head == NULL;
}