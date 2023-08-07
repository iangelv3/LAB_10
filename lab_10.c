#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int count; // Number of occurrences
};

struct Trie
{
    struct TrieNode *root;
};

// Create a new TrieNode
struct TrieNode *createTrieNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        for (int i = 0; i < ALPHABET_SIZE; ++i)
            node->children[i] = NULL;
        node->count = 0;
    }
    return node;
}

// Initialize a new Trie
struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    if (trie)
    {
        trie->root = createTrieNode();
    }
    return trie;
}

// Insert a word into the Trie
void insert(struct Trie *pTrie, char *word)
{
    if (!pTrie || !word)
        return;

    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
            current->children[index] = createTrieNode();
        current = current->children[index];
    }
    current->count++;
}

// Count the number of occurrences of a word in the Trie
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    if (!pTrie || !word)
        return 0;

    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
            return 0; // Word not found
        current = current->children[index];
    }
    return current->count;
}

// Deallocate the Trie
void deallocateTrieNode(struct TrieNode *node)
{
    if (!node)
        return;

    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        deallocateTrieNode(node->children[i]);
    }
    free(node);
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (!pTrie)
        return NULL;

    deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

int readDictionary(char *filename, char ***pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Unable to open the file.\n");
        return 0;
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    char **words = (char **)malloc(numWords * sizeof(char *));
    for (int i = 0; i < numWords; ++i)
    {
        words[i] = (char *)malloc(256 * sizeof(char));
        fscanf(file, "%s", words[i]);
    }

    fclose(file);
    *pInWords = words;
    return numWords;
}

int main(void)
{
    char **inWords;
    int numWords = readDictionary("dictionary.txt", &inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (!pTrie)
        printf("Trie deallocated successfully.\n");

    // Free memory for inWords
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }
    free(inWords);

    return 0;
}
