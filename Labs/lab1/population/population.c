#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Defining variables
    int starting_size;
    int current_size;
    int ending_size;
    int years = 0;

    // Getting starting and ending size of the population
    do
    {
        starting_size = get_int("Starting population size (Number not less than 9): ");
        current_size = starting_size;
    }
    while (starting_size < 9);

    do
    {
        ending_size = get_int("Ending population size (Number not less than %i): ", starting_size);
    }
    while (ending_size < starting_size);


    // Calculates the number of years needed to get to the ending population or a population greater than that
    while (current_size < ending_size)
    {
        current_size += starting_size / 3;
        current_size -= starting_size / 4;
        years++;
        starting_size = current_size;
    }

    // Printing the answer to the user
    printf("Years: %i\n", years);

}