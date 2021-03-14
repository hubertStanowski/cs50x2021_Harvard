#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Prototypes
int compute_score(string word);

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};


int main(void)
{
    // Getting words from the players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Determining who is the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }


}


int compute_score(string word)
{
    int idx, sum = 0;
    char ch;

    // Checking whether every character is a letter and summarizing the points
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        ch = toupper(word[i]);

        if (ch >= 'A' && ch <= 'Z')
        {
            idx = (int) ch - 65;
            sum += POINTS[idx];
        }

    }

    return sum;


}