#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include "src/utils.h"
#include "src/hashtable.h"

//Global Variables for transasct
transaction transaction_arr[50];
int transaction_arr_ptr = 0;

// Blockchain
Block *BlockChain;

//Global Variables for User
HashTable usertable;

//Functions

void transact(int sender, int reciever, double amount)
{
    if (user_arr[sender].joinDateTime[0] == 0 && user_arr[sender].joinDateTime[1] == 0)
    {
        printf("Sender doesn't exist\n");
        return;
    }
    if (sender == reciever)
    {
        printf("Sender and reciver can't be the same\n");
        return;
    }
    if (user_arr[reciever].joinDateTime[0] == 0 && user_arr[reciever].joinDateTime[1] == 0)
    {
        printf("Reciever doesn't exist\n");
        return;
    }
    if (user_arr[sender].balance < amount)
    {
        printf("Amount can't be greater than sender's balance");
        return;
    }
    if (amount == 0)
    {
        printf("Amount can't be zero\n");
        return;
    }
    if (amount < 0)
    {
        printf("amount can't be negative\n");
        return;
    }
    update(sender, reciever, amount);
    if (transaction_arr_ptr == 50)
    {
        createBlock();
        for (int i = 0; i < 50; i++)
        {
            transaction_arr[i].SenderID = 0;
            transaction_arr[i].RecieverID = 0;
            transaction_arr[i].Amount = 0;
        }
        transaction_arr_ptr = 0;
    }
    else
    {
        transaction_arr_ptr += 1;
    }
    return;
}

//Updates values in respective arrays
void update(int sender, int reciever, double amount)
{

    //Updates the values in the array of transactions for the current block
    transaction_arr[transaction_arr_ptr].SenderID = sender;
    transaction_arr[transaction_arr_ptr].RecieverID = reciever;
    transaction_arr[transaction_arr_ptr].Amount = amount;

    //Updates the transaction array of the sender
    user_arr[sender].transactions[user_arr[sender].numTransactions].SenderID = sender;
    user_arr[sender].transactions[user_arr[sender].numTransactions].RecieverID = reciever;
    user_arr[sender].transactions[user_arr[sender].numTransactions].Amount = amount;

    //Updates sender balance
    user_arr[sender].balance -= amount;

    //Updates transaction array of the sender
    user_arr[reciever].transactions[user_arr[reciever].numTransactions].SenderID = sender;
    user_arr[reciever].transactions[user_arr[reciever].numTransactions].RecieverID = reciever;
    user_arr[reciever].transactions[user_arr[reciever].numTransactions].Amount = amount;

    //Updates reciever balance
    user_arr[reciever].balance += amount;

    //Updates number of transactions for the sender and reciever
    user_arr[sender].numTransactions += 1;
    user_arr[reciever].numTransactions += 1;
}

void inqure_bal(int user)
{
    if (user_arr[user].joinDateTime[0] == 0 && user_arr[user].joinDateTime[1] == 0)
    {
        printf("User doesn't exist");
        return;
    }
    printf("The balance for the user with UID %d is %lf units\n", user, user_arr[user].balance);
    return;
}

void inquire_transactions(int user)
{
    for (int i = 0; i < user_arr[user].numTransactions; i++)
    {
        printf("%i - Sender: %d \t Reciever: %d \t Amount: %lf", i + 1, user_arr[user].transactions[i].SenderID, user_arr[user].transactions[i].RecieverID, user_arr[user].transactions[i].Amount);
    }
    return;
}

void addUser(HashTable *T, Person elem)
{

    int r = 0;
    Person user = (Person)malloc(sizeof(Person));

    srand(time(0));
    while (1)
    {
        r = rand();

        if (SearchID(usertable, r) != -1)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    user.uID = r;
    time(&user.joinDateTime);
    user.balance = 1000;
    user.numTransactions = 0;
    printf("Your uID is %d\n", user.uID);
    InsertS(usertable, user);
}

void createBlock()
{
    Block *newBlock = (*Block)malloc(sizeof(Block));
    Block *lastBlock = BlockChain;

    while (lastBlock->NextBlock)
    {
        lastBlock = lastBlock->NextBlock;
    }

    newBlock->BlockNumber = lastBlock->BlockNumber + 1;
    newBlock->NextBlock = NULL;
    newBlock->Nonce = rand() % 500 + 1;

    newBlock->Transactions = (transaction *)malloc(50 * sizeof(transaction));
    if (newBlock->Transactions == NULL)
    {
        printf("Insfficient space for new block! Terminating program\n");
        exit(0);
    }
    for (int i = 0; i < 50; i++)
        newBlock->Transactions[i] = transaction_arr[i];

    getBlockHash(newBlock->PrevBlockHash, lastBlock->BlockNumber,
                 newBlock->Transactions, lastBlock->PrevBlockHash, newBlock->Nonce);
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
    for (int i = 0; i < 50; i++)
    {
        snprintf(tempTransact, sizeof(tempTransact), "{%d, %d, %d}",
                 transactions[i].SenderID, transactions[i].Amount, transactions[i].RecieverID);
        strcat(dest, tempTransact);
    }
}

// calculates hash of a block by taking destination string and block contents as arguments
void getBlockHash(char *hash, int blocknum, transaction *transactions, char *PrevBlockHash, int nonce)
{
    char tempBuffer[2000];
    char transactionStr[1000];

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