#include <stdlib.h>
#include "utils.h"
#include "hashtable.h"
#include <stdio.h>

Node *makenode(Person n)
{
    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->user = n;
    newnode->next = NULL;
    return newnode;
}

void initHashTable(int m)
{
    B.userTable.num_elems = 0;
    B.userTable.tsize = m;
    B.userTable.table = (Node *)malloc(m * sizeof(Node));
    if (B.userTable.table == NULL)
    {
        printf("Error: Insufficient memory\n");
        exit(0);
    }

    for (int i = 0; i < m; i++)
    {
        B.userTable.table[i].next = NULL;
    }
}

void DeleteHashTable()
{
    int n = B.userTable.tsize;
    for (int i = 0; i < n; i++)
    {
        Node *cur = B.userTable.table[i].next;
        while (cur)
        {
            Node *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(B.userTable.table);
}

void InsertS(Person elem)
{
    int hash = elem.uID % B.userTable.tsize;
    Node *newnode = makenode(elem);
    Node *temp = &(B.userTable.table[hash]);

    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = newnode;
    B.userTable.num_elems++;
}

Person *FindUser(int uID)
{
    int hash = uID % B.userTable.tsize;
    Node *temp = B.userTable.table[hash].next;

    for (; temp != NULL; temp = temp->next)
    {
        if (temp->user.uID == uID)
        {
            return &(temp->user);
        }
    }
    return NULL;
}