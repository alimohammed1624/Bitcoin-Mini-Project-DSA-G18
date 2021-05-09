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
    Person user;
    Node *next;
};

void initHashTable(int m);
void DeleteHashTable();
void InsertS(Person elem);
Person *FindUser(int uID);

#endif