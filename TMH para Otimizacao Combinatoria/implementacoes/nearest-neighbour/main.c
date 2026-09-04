#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int Node;
typedef Node* Candidates;

typedef struct graph
{
    Node** values;
    unsigned short int size;
} Graph;

Node *allocNode(unsigned short int size)
{
    Node *n = (Node*) malloc(sizeof(Node) * size);

    return n;
}

Node** allocGraphMatrix(unsigned short int size)
{
    Node **n = (Node**) malloc(sizeof(Node*) * size);

    if(n == NULL)
        return NULL;

    for(unsigned short int i = 0 ; i < size ; i++)
    {
        n[i] = allocNode(size);

        // In case some allocation does not work out
        if(n[i] == NULL)
        {
            for(unsigned short int j = i - 1 ; j >= 0 ; j--)
                free(n[i]);

            free(n);
        }
    }

    return n;
}

void deallocGraphMatrix(Graph *g)
{
    for(unsigned short int i = 0 ; i < g->size ; i++)
        free(g->values[i]);

    free(g->values);
}

void printGraph(Graph *g)
{
    for(unsigned short int i = 0 ; i < g->size ; i++)
    {
        for(unsigned short int j = 0 ; j < g->size ; j++)
            printf("%hu ", g->values[i][j]);

        printf("\n");
    }
}

void fillGraphMatrix(Graph *g, unsigned short int size)
{
    int ind = 0;

    for(unsigned short int i = 0 ; i < g->size ; i++)
    {
        for(unsigned short int j = 0 ; j < g->size ; j++)
        {
            ind++;
            g->values[i][j] = ind;
        }
    }
}

Candidates allocCandidates(unsigned short int size)
{   
    Candidates c = (Candidates) malloc(sizeof(Candidates) * size);

    return c;
}

void deallocCandidates(Candidates *c)
{   
    free(*c);
}

void printCandidates(Candidates c, unsigned short int size)
{
    for(unsigned short int i = 0 ; i < size ; i++)
        printf("%d ", c[i]);

    printf("\n");
}

void fillCandidates(Candidates c, unsigned short int size)
{
    for(unsigned short int i = 0 ; i < size ; i++)
        c[i] = i;
}

/*
    It takes the candidate in "index" position and "removes" it from the set
    The return is the removed candidate
*/
Node selectAndRemoveCandidate(Candidates c, unsigned short int size, unsigned short int index)
{
    Node candidate = c[index];
    Node aux;

    aux = c[size - 1];
    c[size - 1] = c[index];
    c[index] = aux;

    return candidate;
}

Node getSmallest(Graph *g, Candidates c, unsigned short int set_size, Node reference)
{
    Node smallest;
    Node aux;
    unsigned short int idx_smallest;

    smallest = c[0];
    idx_smallest = 0;

    for(unsigned short int i = 1 ; i < set_size ; i++)
    {
        aux = c[i];

        if(g->values[reference][aux] < g->values[reference][smallest])
        {
            smallest = aux;
            idx_smallest = i;
        }
    }

    return idx_smallest;
}

int main()
{
    Graph g;
    Candidates c;
    Node next;
    Node idx_smallest;
    unsigned short int size = 6;
    unsigned short int set_size = size;

    g.size = size;
    g.values = allocGraphMatrix(g.size);
    c = allocCandidates(g.size);

    if(g.values == NULL || c == NULL)
        return 1;

    // Seed to generate random numbers
    srand((unsigned int) time(NULL));

    // Insert predictable data inside the data structures to run/test immediatelly
    fillCandidates(c, size);
    fillGraphMatrix(&g, size);

    next = selectCandidate(c, set_size, rand() % set_size);
    set_size--;

    while(set_size != 0)
    {
        idx_smallest = getSmallest(&g, c, set_size, next);
        next = selectAndRemoveCandidate(c, set_size, idx_smallest);
        set_size--;
    }

    printGraph(&g);
    printCandidates(c, g.size);
    deallocGraphMatrix(&g);
    deallocCandidates(&c);

    return 0;
}