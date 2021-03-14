#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

const int BLOCKSIZE = 512;


int main(int argc, char *argv[])
{
    // Checking whether user inputed correct number of command line arguments
    if (argc != 2)
    {
        printf("Usage: filename\n");
        return 1;
    }

    // Opening the input file
    char *infile = argv[1];
    FILE *input = fopen(infile, "r");

    // Checking whether the input file can be opened
    if (input == NULL)
    {
        printf("Could not open the file, usage: filename\n");
        return 1;
    }

    // Declaring variables
    int file_count = 0;
    char *filename = malloc(3 * sizeof(int));

    BYTE block [BLOCKSIZE];

    while (fread(block, 1, BLOCKSIZE, input) == BLOCKSIZE)
    {

        // Checking whether the block is the start of a new file
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[3] <= 0xef)
        {
            if (file_count < 10)
            {
                sprintf(filename, "00%i.jpg", file_count);
            }
            else
            {
                sprintf(filename, "0%i.jpg", file_count);
            }

            FILE *output = fopen(filename, "w");
            fwrite(block, 1, BLOCKSIZE, output);
            fclose(output);

            file_count++;
        }

        // Appending to the current file
        else if (file_count != 0)
        {
            FILE *output = fopen(filename, "a");
            fwrite(block, 1, BLOCKSIZE, output);
            fclose(output);
        }
    }


    fclose(input);


    return 0;
}