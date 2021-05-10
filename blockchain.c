#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/utils.h"
#include "src/hashtable.h"

// helper functions to find block hash and original nonce of attacked block
// defined in "blockhash.c"
int getNonce(unsigned char *curHash, unsigned char *originalHash, int nonce);
void getBlockHash(unsigned char *hash, int blocknum, transaction *transactions, unsigned char *PrevBlockHash, int nonce);

void initBlockChain()
{
    B.Bchain = NULL;
    for (int i = 0; i < 20; i++)
        B.LastBlockHash[i] = 0;
    B.LastBlockHash[20] = '\0';
    B.numBlocks = 0;
    B.transaction_arr_ptr = 0;
    initHashTable(1009);
}

void deleteBlockChain()
{
    Block *cur = B.Bchain;
    while (cur != NULL)
    {
        Block *temp = cur;
        cur = cur->NextBlock;
        free(temp);
    }
    B.Bchain = NULL;
    DeleteHashTable();
}

// Updates values in respective arrays
void update(Person *sender, Person *reciever, double amount)
{
    transaction tr;
    tr.Amount = amount;
    tr.SenderID = sender->uID;
    tr.RecieverID = reciever->uID;

    // Updates the values in the array of transactions for the current block
    B.transaction_arr[B.transaction_arr_ptr] = tr;
    B.transaction_arr_ptr++;

    // Updates the transaction for the sender
    sender->transactions[sender->numTransactions] = tr;
    sender->numTransactions++;
    sender->balance -= amount;

    // Updates transaction for the reciever
    reciever->transactions[reciever->numTransactions] = tr;
    reciever->numTransactions++;
    reciever->balance += amount;
}

// verifies if the requested transaction if valid and adds it to the list of transactions
// and updates sender and reviever info by calling update()
// if number of new transactions is num_t, it creates a new block and adds it to the blockchain
void transact(int sender, int reciever, double amount)
{
    Person *S = FindUser(sender);
    Person *R = FindUser(reciever);

    if (S == NULL)
    {
        printf("Transaction aborted: Sender ID invalid\n");
        return;
    }
    if (R == NULL)
    {
        printf("Transaction aborted: Reciever ID invalid\n");
        return;
    }
    if (S == R)
    {
        printf("Transaction aborted: Sender and reciver must be different\n");
        return;
    }
    if (S->balance < amount)
    {
        printf("Transaction aborted: Sender balance insufficient\n");
        return;
    }
    if (amount <= 0)
    {
        printf("Transaction aborted: Amount must be greater than zero\n");
        return;
    }
    update(S, R, amount);

    if (B.transaction_arr_ptr == num_t)
    {
        createBlock();
        for (int i = 0; i < num_t; i++)
        {
            B.transaction_arr[i].SenderID = 0;
            B.transaction_arr[i].RecieverID = 0;
            B.transaction_arr[i].Amount = 0;
        }
        B.transaction_arr_ptr = 0;
    }
    printf("Transaction successful!\n");
    return;
}

// adds a new user and displays their user id
void addUser()
{
    int r = 0;
    Person user;

    while (1)
    {
        r = rand();

        if (FindUser(r) == NULL)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    user.uID = r;
    time_t rawtime = time(NULL);
    user.joinDateTime = *localtime(&rawtime);
    user.balance = 1000;
    user.numTransactions = 0;
    printf("New user added: User ID is %d\n", user.uID);
    InsertS(user);
}

void createBlock()
{
    Block *newBlock = (Block *)malloc(sizeof(Block));
    if (newBlock == NULL)
    {
        printf("Error: Insfficient space\n");
        exit(0);
    }

    // initialize the new block
    newBlock->BlockNumber = B.numBlocks + 1;
    strcpy(newBlock->PrevBlockHash, B.LastBlockHash);
    newBlock->NextBlock = NULL;
    newBlock->Nonce = rand() % 500 + 1;
    for (int i = 0; i < num_t; i++)
        newBlock->Transactions[i] = B.transaction_arr[i];

    // Append new block to the existing chain
    if (B.Bchain == NULL)
        B.Bchain = newBlock;
    else
    {
        Block *lastBlock = B.Bchain;
        while (lastBlock->NextBlock)
            lastBlock = lastBlock->NextBlock;
        lastBlock->NextBlock = newBlock;
    }

    // update Block chain deatils
    B.transaction_arr_ptr = 0;
    B.numBlocks++;
    getBlockHash(B.LastBlockHash, newBlock->BlockNumber, newBlock->Transactions, newBlock->PrevBlockHash, newBlock->Nonce);
}

void attack()
{
    if (B.Bchain == NULL)
    {
        printf("Attack not possible, block chain empty\n");
        return;
    }

    int randNum = rand() % 50 + 1;
    printf("Random number generated: %d\n", randNum);

    if (randNum > B.numBlocks)
    {
        printf("Attack failed: Block with number %d does not exist\n", randNum);
        return;
    }

    Block *curBlock = B.Bchain;
    while (curBlock != NULL)
    {
        if (curBlock->BlockNumber == randNum)
        {
            int originalNonce = curBlock->Nonce;
            curBlock->Nonce = rand() % 500 + 1;
            printf("Attack successful! Nonce modified\n");
            printf("Original nonce: %d\nNew nonce:%d\n", originalNonce, curBlock->Nonce);
            return;
        }
        curBlock = curBlock->NextBlock;
    }
}

void validate()
{
    if (B.Bchain == NULL)
    {
        printf("Block chain empty\n");
        return;
    }

    Block *curBlock = B.Bchain;
    Block *nextBlock;
    int countAttacks = 0;
    while (curBlock->NextBlock != NULL)
    {
        unsigned char curHash[30];
        nextBlock = curBlock->NextBlock;

        // get the hash of the current block
        getBlockHash(curHash, curBlock->BlockNumber, curBlock->Transactions,
                     curBlock->PrevBlockHash, curBlock->Nonce);

        // compare hash of current block with prevBlockHash of next block
        if (strncmp(curHash, nextBlock->PrevBlockHash, 20) != 0)
        {
            if (countAttacks == 0)
                printf("Attack detected!\n");
            int curNonce = curBlock->Nonce;
            curBlock->Nonce = getNonce(curHash, nextBlock->PrevBlockHash, curBlock->Nonce);

            printf("Block number: %d\n", curBlock->BlockNumber);
            printf("Current nonce: %d\nOriginal nonce: %d\n\n", curNonce, curBlock->Nonce);
            countAttacks++;
        }
        curBlock = curBlock->NextBlock;
    }

    // validating last block
    if (curBlock->NextBlock == NULL)
    {
        unsigned char curHash[30];

        // get the hash of the current block
        getBlockHash(curHash, curBlock->BlockNumber, curBlock->Transactions,
                     curBlock->PrevBlockHash, curBlock->Nonce);

        // compare hash of current block with prevBlockHash of next block
        if (strncmp(curHash, B.LastBlockHash, 20) != 0)
        {
            if (countAttacks == 0)
                printf("Attack detected!\n");
            int curNonce = curBlock->Nonce;
            curBlock->Nonce = getNonce(curHash, B.LastBlockHash, curBlock->Nonce);

            printf("Block number: %d\n", curBlock->BlockNumber);
            printf("Current nonce: %d\nOriginal nonce: %d\n", curNonce, curBlock->Nonce);
            countAttacks++;
        }
    }

    printf("Block chain validated: %d attacks have been reversed\n", countAttacks);
}