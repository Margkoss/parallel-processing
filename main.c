#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* RED = "\033[0;31m";
const int STEP_SIZE = 100;

char** loadFile(char* filename, int* len);

int main(int argc, char* argv[])
{

    if(argc == 1)
    {
        fprintf(stderr, "%sExitting...\nNo filename was given\n", RED);
        exit(1);
    }

    //Reading the file
    //Define a length for the file to keep track
    int length = 0;
    //Load file into memorry
    char **words = loadFile(argv[1], &length);

    printf("%s\n", words[0]);
    return 0;
}

char** loadFile(char* filename, int* len)
{
    // Open file for reading
    FILE *f = fopen(filename, "r");
    if(!f)
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
    while(fgets(buf, 200, f))
    {
        //Ignore all lines begining with %
        if(buf[0] == '%')
        {
            continue;
        }
        //Check if the lines array is full, and extend as needed
        if(i >= arrLength)
        {
            arrLength += STEP_SIZE;
            char **newlines = realloc(lines, arrLength * sizeof(char*));
            if(!newlines) {
                fprintf(stderr, "%sExitting...\nCould not read entire file\n", RED);
                exit(1);
            }
            lines = newlines;
        }

        //Trim off new line char at end of line
        buf[strlen(buf) - 1] = '\0';

        //Get the length of the buffer and allocate memory for the line
        char* str = (char *)malloc((strlen(buf) + 1));
        strcpy(str, buf);
        lines[i] = str;
        i++;
    }

    *len = i;
    return lines;
}
