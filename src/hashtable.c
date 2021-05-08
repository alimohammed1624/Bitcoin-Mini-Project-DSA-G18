#include <stdlib.h>
#include "utils.h"
#include "hashtable.h"

Node *makenode(Person n)
{
    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->user = n;
    newnode->next = NULL;
    return newnode;
}

void initHashTable(int m)
{
    userTable.num_elems = 0;
    userTable.tsize = m;
    userTable.table = (Node *)malloc(m * sizeof(Node));
    if (userTable.table == NULL)
    {
        printf("Error: Insufficient memory\n");
        exit(0);
    }

    for (int i = 0; i < m; i++)
    {
        userTable.table[i].next = NULL;
    }
    return userTable;
}

void DeleteHashTable()
{
    int n = userTable.tsize;
    for (int i = 0; i < n; i++)
    {
        Node *cur = userTable.table[i].next;
        while (cur)
        {
            Node *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(userTable.table);
}

void InsertS(Person elem)
{
    int hash = elem.uID % userTable.tsize;
    Node *newnode = makenode(elem);
    Node *temp = &userTable.table[hash];

    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = newnode;
    userTable.num_elems++;
}

Person *FindUser(int uID)
{
    int hash = uID % userTable.tsize;
    Node *temp = userTable.table[hash].next;

    for (; temp != NULL; temp = temp->next)
    {
        if (temp->user.uID == uID)
        {
            return &(temp->user);
        }
    }
    return NULL;
}