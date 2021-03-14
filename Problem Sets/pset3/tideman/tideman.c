#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    int strength;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count ; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }

}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    for (int i = 0; i < MAX - 1; i++)
    {
        for (int j = i + 1; j < MAX; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    // iterate i = 0 , j = i + 1, [i][j] vs [j][i] bigger -> winner, smaller -> loser
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // highest
    int max_idx = 0;
    pair a;
    for (int i = 0, n = pair_count - 1; i < n ; i++)
    {
        pairs[i].strength = preferences[pairs[i].winner][pairs[i].loser];
    }

    for (int i = 0, n = pair_count - 1; i < n ; i++)
    {
        max_idx = i;
        for (int j = i + 1 ; j < n ; j++)
        {
            if (pairs[j].strength > pairs[max_idx].strength)
            {
                max_idx = j;
            }
        }

        a = pairs[i];
        pairs[i] = pairs[max_idx];
        pairs[max_idx] = a;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int winner, loser;

    for (int i = 0; i < pair_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;

        if (!cycle(winner, loser))
        {
            locked[winner][loser] = true;
        }


    }

    return;
}


// Checks whether there is a cycle if we add a pair containing certain winner and loser
bool cycle(int winner, int loser)
{
    // if loser wins with the winner then there is a cycle
    if (locked[loser][winner] == true)
    {
        return true;
    }

    // For every candidate we check whether they win with the winner
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[j][winner] == true)
        {
            // If they do win then we check if they create a cicle with the loser
            return cycle(j, loser);
            // If they will lose with the loser that means that there is a circle
            // Winner --> Loser --> j --> winner
        }
    }
    return false;

}

// Print the winner of the election
void print_winner(void)
{
    bool winner;
    for (int i = 0; i < candidate_count; i++)
    {
        winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                winner = false;
            }
        }

        if (winner == true)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}



