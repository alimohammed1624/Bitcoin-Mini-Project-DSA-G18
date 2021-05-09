#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include "src/utils.h"
#include "src/hashtable.h"

void initBlockChain()
{
    BlockChain = NULL;
    initHashTable(1009);
}

void deleteBlockChain()
{
    Block *cur = BlockChain;
    while (cur != NULL)
    {
        Block *temp = cur;
        cur = cur->NextBlock;
        free(temp);
    }
    BlockChain = NULL;
    DeleteHashTable();
}

void transact(int sender, int reciever, double amount)
{
    Person *S = FindUser(sender);
    Person *R = FindUser(reciever);

    if (S == NULL)
    {
        printf("Error: Sender ID invalid\n");
        return;
    }
    if (R == NULL)
    {
        printf("Error: Reciever ID invalid\n");
        return;
    }
    if (S == R)
    {
        printf("Error: Sender and reciver must be different\n");
        return;
    }
    if (S->balance < amount)
    {
        printf("Sender balance insufficient");
        return;
    }
    if (amount <= 0)
    {
        printf("Error: Amount must be greater than zero\n");
        return;
    }
    update(S, R amount);

    if (transaction_arr_ptr == num_t)
    {
        createBlock();
        for (int i = 0; i < num_t; i++)
        {
            transaction_arr[i].SenderID = 0;
            transaction_arr[i].RecieverID = 0;
            transaction_arr[i].Amount = 0;
        }
        transaction_arr_ptr = 0;
    }
    return;
}

//Updates values in respective arrays
void update(Person *sender, Person *reciever, double amount)
{
    transaction tr;
    tr.Amount = amount;
    tr.SenderID = sender->uID;
    tr.RecieverID = reciever->uID;

    //Updates the values in the array of transactions for the current block
    transaction_arr[transaction_arr_ptr] = tr;
    transaction_arr_ptr++;

    //Updates the transaction for the sender
    sender->transactions[sender->numTransactions] = tr;
    sender->numTransactions++;
    sender->balance -= amount;

    //Updates transaction for the reciever
    reciever->transactions[reciever->numTransactions] = tr;
    reciever->numTransactions++;
    reciever->balance += amount;
}

void inqure_bal(int user)
{
    Person *P = FindUser(user);
    if (P == NULL)
    {
        printf("Invalid user ID");
        return;
    }
    printf("The balance for the user with UID %d is %lf units\n", P->uID, P->balance);
    return;
}

void inquire_transactions(int user)
{
    Person *P = FindUser(user);
    for (int i = 0; i < user_arr[user].numTransactions; i++)
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
    printf("New user added: User iD is %d, %d\n", user.uID);
    InsertS(user);
}

void createBlock()
{
    Block *newBlock = (*Block)malloc(sizeof(Block));
    Block *lastBlock = BlockChain;
    if (newBlock == NULL)
    {
        printf("Error: Insfficient space\n");
        exit(0);
    }

    if (BlockChain == NULL)
    {
        newBlock->BlockNumber = 1;
        strcpy(newBlock->PrevBlockHash, "0");
    }
    else
    {
        while (lastBlock->NextBlock)
        {
            lastBlock = lastBlock->NextBlock;
        }
        newBlock->BlockNumber = lastBlock->BlockNumber + 1;
        getBlockHash(newBlock->PrevBlockHash, lastBlock->BlockNumber,
                     newBlock->Transactions, lastBlock->PrevBlockHash, newBlock->Nonce);
    }
    newBlock->NextBlock = NULL;
    newBlock->Nonce = rand() % 500 + 1;
    for (int i = 0; i < num_t; i++)
        newBlock->Transactions[i] = transaction_arr[i];

    transaction_arr_ptr = 0;
    lastBlock->NextBlock = newBlock;
}

void attack()
{
    if (BlockChain == NULL)
    {
        printf("Attack not possible, block chain empty\n");
        return;
    }

    int randNum = rand() % 50 + 1;
    printf("Random number generated:%d\n", randNum);
    Block *curBlock = BlockChain;
    while (curBlock != NULL)
    {
        if (curBlock->BlockNumber == randNum)
        {
            curBlock->Nonce = rand() % 500 + 1;
            printf("Attack successful! Nonce modified\n");
            return;
        }
        else if (curBlock->BlockNumber > randNum)
            break;

        curBlock = curBlock->NextBlock;
    }
    printf("Attack failed: Block with number %d does not exist\n", randNum);
    return;
}

// concatenates the transaction array into string dest
void getTranactionStr(char *dest, transaction *transactions)
{
    char tempTransact[50];
    dest[0] = '\0';
    for (int i = 0; i < num_t; i++)
    {
        snprintf(tempTransact, sizeof(tempTransact), "{%d, %d, %d}",
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
    if (BlockChain == NULL)
    {
        printf("Block chain empty\n");
        return;
    }
    if (BlockChain->NextBlock == NULL)
    {
        printf("Block chain validated");
        return;
    }

    Block *curBlock = BlockChain->NextBlock;
    Block *prevBlock = BlockChain;
    int countAttacks = 0;
    while (curBlock != NULL)
    {
        char curHash[500];
        getBlockHash(curHash, curBlock->BlockNumber, curBlock->Transactions,
                     curBlock->PrevBlockHash, prevBlock->Nonce);
        if (strcmp(curHash, curBlock->PrevBlockHash) != 0)
        {
            prevBlock->Nonce = getNonce(curHash, curBlock->PrevBlockHash, prevBlock->Nonce);
            countAttacks++;
        }

        prevBlock = curBlock;
        curBlock = curBlock->NextBlock;
    }

    printf("Block chain validated: %d attacks detected\n", countAttacks);
}