#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Asks for a name and then greets using given name
    string name = get_string("What is your name>\n");
    printf("hello, %s\n", name);
}