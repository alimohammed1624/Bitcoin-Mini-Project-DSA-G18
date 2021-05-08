#ifndef HASHTABLE_H_
#define HASHTABLE_H_

typedef struct hashtable HashTable;
typedef struct node Node;
struct hashtable
{
    int tsize;
    int num_elems;
    Node *table;
};

struct node
{
    struct person element;
    Node *next;
};

HashTable CreateHashTable(int m);
Node *makenode(struct person n);
void DeleteHashTable(HashTable T);
void InsertS(HashTable *T, struct person elem);
struct person *SearchS(HashTable T, struct person elem);
int SearchID(HashTable T, int elem);
#endif