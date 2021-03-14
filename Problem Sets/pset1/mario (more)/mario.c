#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    string row;
    // Getting a heght of the pyramid which is a number between 1 and 8 (rejects everything else)
    do
    {
        height = get_int("Enter the height (number between 1 and 8): ");
    }
    while (8 < height || height < 1);


    // Printing the pyramid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (height - i - 1); j++)
        {
            printf(" ");
        }

        // Printing the first part
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }

        printf("  ");

        //Printing the second part
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }

        printf("\n");
    }


}