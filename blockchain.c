#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include "src/utils.h"
#include "src/hashtable.h"

void initBlockChain()
{
    B.Bchain = NULL;
    strcpy(B.LastBlockHash, "0");
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

//Updates values in respective arrays
void update(Person *sender, Person *reciever, double amount)
{
    transaction tr;
    tr.Amount = amount;
    tr.SenderID = sender->uID;
    tr.RecieverID = reciever->uID;

    //Updates the values in the array of transactions for the current block
    B.transaction_arr[B.transaction_arr_ptr] = tr;
    B.transaction_arr_ptr++;

    //Updates the transaction for the sender
    sender->transactions[sender->numTransactions] = tr;
    sender->numTransactions++;
    sender->balance -= amount;

    //Updates transaction for the reciever
    reciever->transactions[reciever->numTransactions] = tr;
    reciever->numTransactions++;
    reciever->balance += amount;
}

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
    return;
}

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
        printf("%i - Sender: %d \t Reciever: %d \t Amount: %lf",
               i + 1, P->transactions[i].SenderID, P->transactions[i].RecieverID, P->transactions[i].Amount);
    }
    return;
}

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

// concatenates the transaction array into string dest
void getTranactionStr(char *dest, transaction *transactions)
{
    char tempTransact[50];
    dest[0] = '\0';
    for (int i = 0; i < num_t; i++)
    {
        snprintf(tempTransact, sizeof(tempTransact), "{%d, %lf, %d}",
                 transactions[i].SenderID, transactions[i].Amount, transactions[i].RecieverID);
        strcat(dest, tempTransact);
    }
}

// calculates hash of a block by taking destination string and block contents as arguments
void getBlockHash(char *hash, int blocknum, transaction *transactions, char *PrevBlockHash, int nonce)
{
    char tempBuffer[50 * num_t + 300];
    char transactionStr[50 * num_t];

    getTranactionStr(transactionStr, transactions);
    snprintf(tempBuffer, sizeof(tempBuffer), "%d, %s, %s", blocknum, PrevBlockHash, transactionStr);

    SHA1(tempBuffer, strlen((const char *)tempBuffer), hash);
    hash[1] ^= nonce;
    hash[0] ^= nonce >> 8;
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
            curBlock->Nonce = rand() % 500 + 1;
            printf("Attack successful! Nonce modified\n");
            return;
        }
        curBlock = curBlock->NextBlock;
    }
}

// returns the original nonce of a block
int getNonce(char *curHash, char *originalHash, int nonce)
{
    curHash[1] ^= nonce;
    curHash[0] ^= nonce >> 8;

    curHash[0] ^= originalHash[0];
    curHash[1] ^= originalHash[1];

    int originalNonce = (int)curHash[0];
    originalNonce = originalNonce << 8;
    originalNonce += (int)curHash[1];
    return originalNonce;
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
    while (curBlock != NULL)
    {
        char curHash[500];
        nextBlock = curBlock->NextBlock;

        // get the hash of the current block
        getBlockHash(curHash, curBlock->BlockNumber, curBlock->Transactions,
                     curBlock->PrevBlockHash, curBlock->Nonce);

        // compare hash of current block with prevBlockHash of next block
        if (strcmp(curHash, nextBlock->PrevBlockHash) != 0)
        {
            if (curBlock->NextBlock == NULL)
                curBlock->Nonce = getNonce(curHash, B.LastBlockHash, curBlock->Nonce);
            else
                curBlock->Nonce = getNonce(curHash, nextBlock->PrevBlockHash, curBlock->Nonce);
            countAttacks++;
        }

        curBlock = curBlock->NextBlock;
    }

    printf("Block chain validated: %d attacks detected\n", countAttacks);
}