#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int main(void)
{
    // Defining variables and getting the input text
    string text = get_string("Text: ");
    int words = 0, letters = 0, sentences = 0;
    int grade;
    char ch;
    float L, S, index;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        ch = toupper(text[i]);

        // Checking whether a character is a letter
        if (ch >= 'A' && ch <= 'Z')
        {
            letters++;
        }

        // Checking whether it is a punctutation mark or a space
        else if (ch == '!' || ch == '?' || ch == '.' || ch == ' ')
        {
            if (ch != ' ')
            {
                sentences++;
            }
            words++;
        }
    }

    // Accounting for the fact that after punctuation there are spaces except for the very last one (-1)
    words -= (sentences - 1) ;

    // Calculating ratios needed for the index
    L = (100 * (float) letters / (float) words);
    S = (100 * (float) sentences / (float) words);

    // Calculating the index
    index = 0.0588 * L - 0.296 * S - 15.8;

    grade = round(index);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}