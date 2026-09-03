#include <stdio.h>
#include <stdlib.h>

typedef int Node;
typedef int* Candidates;

typedef struct nodes
{
    Node** nodes;
    unsigned short int amount;
} Nodes;

Node** allocNodes(unsigned short int amount)
{
    Node **n = (Node**) malloc(sizeof(Node*) * amount);

    if(n == NULL)
        return NULL;

    for(unsigned short int i = 0 ; i < amount ; i++)
    {
        n[i] = (Node*) malloc(sizeof(Node) * amount);

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

void deallocNodes(Nodes* n) {
    for(unsigned short int i = 0 ; i < n->amount ; i++)
        free(n->nodes[i]);

    free(n->nodes);
}

void printNodes(Nodes *n)
{
    for(unsigned short int i = 0 ; i < n->amount ; i++)
    {
        for(unsigned short int j = 0 ; j < n->amount ; j++)
            printf("%hu ", n->nodes[i][j]);

        printf("\n");
    }
}

Candidates allocCandidates(Nodes *n)
{   
    Candidates c = (Candidates) malloc(sizeof(Candidates) * n->amount);

    return c;
}

int main()
{
    Nodes n;
    int ind = 0;

    n.amount = 6;
    n.nodes = allocNodes(n.amount);

    if(n.nodes == NULL)
        return 1;

    for(unsigned short int i = 0 ; i < n.amount ; i++)
    {
        for(unsigned short int j = 0 ; j < n.amount ; j++)
        {
            ind++;
            n.nodes[i][j] = ind;
        }
    }

    printNodes(&n);
    deallocNodes(&n);

    return 0;
}