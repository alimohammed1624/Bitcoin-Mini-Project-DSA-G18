#include <stdio.h>
#include <string.h>
#include "src/utils.h"
#include "src/hashtable.h"

// this file containts all functions to query user/block details

// displays a user's current balance
void inqure_bal(int user)
{
    Person *P = FindUser(user);
    if (P == NULL)
    {
        printf("Invalid user ID\n");
        return;
    }
    printf("The balance for the user with UID %d is %lf units\n", P->uID, P->balance);
    return;
}

// displays a user's transaction history
void inquire_transactions(int user)
{
    Person *P = FindUser(user);
    if (P == NULL)
    {
        printf("Invalid user ID\n");
        return;
    }
    for (int i = 0; i < P->numTransactions; i++)
    {
        printf("%2d - Sender: %9d \t\t Reciever: %9d \t\t Amount: %lf\n",
               i + 1, P->transactions[i].SenderID, P->transactions[i].RecieverID, P->transactions[i].Amount);
    }
    return;
}

// displays the block chain
void displayBlockChain()
{
    printf("Number of blocks: %d\n", B.numBlocks);

    Block *curBlock = B.Bchain;
    while (curBlock)
    {
        printf("Block number: %d\n", curBlock->BlockNumber);
        printf("Nonce: %d\n", curBlock->Nonce);
        printf("Hash of previous block: ");
        // int len = strlen(curBlock->PrevBlockHash);
        for (int i = 0; i < 20; i++)
            printf("%02x ", curBlock->PrevBlockHash[i]);
        printf("\n\n");

        curBlock = curBlock->NextBlock;
    }
}

// displays information about a particular block
void displayBlock(int bnum)
{
    if (B.numBlocks < bnum || bnum < 1)
    {
        printf("Error: block number invalid\n");
        return;
    }

    Block *curBlock = B.Bchain;
    while (curBlock != NULL && curBlock->BlockNumber != bnum)
        curBlock = curBlock->NextBlock;

    printf("Block number: %d\n", curBlock->BlockNumber);
    printf("Nonce: %d\n", curBlock->Nonce);
    printf("Hash of previous block: ");
    // int len = strlen(curBlock->PrevBlockHash);
    for (int i = 0; i < 20; i++)
        printf("%02x ", curBlock->PrevBlockHash[i]);

    printf("\n\nTransactions: \n");
    for (int i = 0; i < num_t; i++)
        printf("%2d - Sender: %9d \t\t Reciever: %9d \t\t Amount: %lf\n",
               i + 1, curBlock->Transactions[i].SenderID, curBlock->Transactions[i].RecieverID,
               curBlock->Transactions[i].Amount);
    printf("\n\n");
}