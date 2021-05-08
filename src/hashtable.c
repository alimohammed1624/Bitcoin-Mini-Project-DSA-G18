#include "utils.h"
#include "hashtable.h"

Node *makenode(Person n)
{
    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->element = n;
    newnode->next = NULL;
    return newnode;
}

HashTable CreateHashTable(int m)
{
    HashTable T;
    T.num_elems = 0;
    T.tsize = m;
    T.table = (Node *)calloc(m, sizeof(Node));
    if (T.table != NULL)
    {
        printf("ERROR");
        exit 0;
    }

    for (int i = 0; i < m; i++)
    {
        T.table[i].element = 0;
        T.table[i].next = NULL;
    }
    return T;
}

void DeleteHashTable(HashTable T)
{
    int n = T.tsize;

    for (int i = 0; i < n; i++)
    {
        Node *cur = T.table[i].next;
        while (cur)
        {
            Node *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(T.table);
}

void InsertS(HashTable *T, Person elem)
{
    int hash = elem.uID % T->tsize;
    Node *newnode = makenode(elem);
    Node *temp = &T->table[hash];

    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = newnode;
    T->num_elems++;
}

Person *SearchS(HashTable T, Person elem)
{
    int hash = elem % T.tsize;
    Person *temp = T.table[hash].next;

    for (int i = 0; temp != NULL; i++, temp = temp->next)
    {
        if (temp->element == elem)
        {
            return temp->element;
        }
    }
    return -1;
}
int SearchID(HashTable T, int elem)
{
    int hash = elem % T.tsize;
    Person *temp = T.table[hash].next;

    for (int i = 0; temp != NULL; i++, temp = temp->next)
    {
        if (temp->element.uID == elem)
        {
            return elem;
        }
    }
    return -1;
}