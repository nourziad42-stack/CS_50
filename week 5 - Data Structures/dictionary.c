// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

// Number of words
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = hash(word);

    node *cursor = table[index];

    while (cursor != NULL)
    {
        int i = 0;

        while (cursor->word[i] != '\0' && word[i] != '\0')
        {
            if (tolower((unsigned char) cursor->word[i]) !=
                tolower((unsigned char) word[i]))
            {
                break;
            }

            i++;
        }

        if (cursor->word[i] == '\0' && word[i] == '\0')
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    for (int i = 0; word[i] != '\0'; i++)
    {
        hash = ((hash << 5) + hash) +
               tolower((unsigned char) word[i]);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%45s", word) == 1)
    {
        node *new_node = malloc(sizeof(node));

        if (new_node == NULL)
        {
            fclose(file);
            unload();
            return false;
        }

        strcpy(new_node->word, word);

        unsigned int index = hash(word);

        new_node->next = table[index];
        table[index] = new_node;

        word_count++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;

            free(temp);
        }

        table[i] = NULL;
    }

    word_count = 0;

    return true;
}
