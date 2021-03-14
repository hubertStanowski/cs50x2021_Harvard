#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Checking whether the user types in the right key
    if (argc != 2)
    {
        printf("./substitution key\n");
        return 1;
    }

    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Defining variables
    string key = argv[1];
    char ch;

    // Checking whether the key is valid
    for (int i = 0, len = strlen(key); i < len; i++)
    {
        ch = toupper(key[i]);
        if (ch < 'A' || ch > 'Z')
        {
            printf("Enter a valid key! \n");
            return 1;
        }

        for (int j = i + 1, n = strlen(key); j < n; j++)
        {
            if (toupper(key[j]) == toupper(key[i]))
            {
                printf("Enter a valid key! \n");
                return 1;
            }
        }
    }

    // Defining rest of the variables
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;
    int idx;



    // Going through all of the characters in the plaintext
    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {
        ch = toupper(plaintext[i]);

        // Checking whether a character is a letter
        if (ch >= 'A' && ch <= 'Z')
        {
            idx = ch - 65;

            // Adding the cyphered letter keeping its case
            if (isupper(plaintext[i]))
            {
                ciphertext [i] = toupper(key[idx]);
            }

            else
            {
                ciphertext [i] = tolower(key[idx]);
            }
        }

    }

    // Printing the ciphered text
    printf("ciphertext: %s\n", ciphertext);

}
