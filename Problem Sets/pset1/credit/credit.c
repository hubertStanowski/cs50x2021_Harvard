#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Declaring variables
    long card_num = get_long("Credit card number: ");
    long start_num = card_num;
    int first = 0;
    int multi = 0, not_multi = 0;
    int product;


    // Applying the Luh's Algorithm
    while (card_num > 0)
    {
        // Adding every other number that we do not multiply
        not_multi += card_num % 10;
        card_num /= 10;

        if (card_num < 100 && card_num > 10)
        {
            first += card_num;
        }

        // Multiplying every other number by two and adding digits of calculated products
        product = (card_num % 10) * 2;
        while (product > 0)
        {
            multi += product % 10;
            product /= 10;
        }
        card_num /= 10;

        if (card_num < 100 && card_num > 10)
        {
            first += card_num;
        }
    }

    // Checking whether the card is valid and if it is what company's card it is
    if ((multi + not_multi) % 10 == 0 && start_num > 1000000000000)
    {
        if (first == 34 || first == 37)
        {
            printf("AMEX\n");
        }

        else if (first == 51 || first == 52 || first == 53 || first == 54 || first == 55)
        {
            printf("MASTERCARD\n");
        }

        else if (first >= 40 && first < 50)
        {
            printf("VISA\n");
        }

        // If no requirements are met, return invalid
        else
        {
            printf("INVALID\n");
        }
    }

    // If no requirements are met, return invalid
    else
    {
        printf("INVALID\n");
    }

}