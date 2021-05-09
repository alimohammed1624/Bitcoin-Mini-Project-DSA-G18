#ifndef UTILS_H_
#define UTILS_H_

#include "hashtable.h"

typedef struct block Block;
typedef struct t transaction;
typedef struct person Person;

// macro for number of transactions in each block
#define num_t 50

// Global variables for transasct
extern transaction transaction_arr[num_t];
extern int transaction_arr_ptr = 0;

// Blockchain
extern Block *BlockChain;

// Global variable for user hashtable
extern HashTable userTable;

struct t
{
    int SenderID;
    int RecieverID;
    double Amount;
};

struct block
{
    int BlockNumber;
    transaction Transactions[num_t];
    char PrevBlockHash[300];
    int Nonce;
    Block *NextBlock;
};

struct person
{
    int uID;
    double balance;
    int numTransactions;
    transaction transactions[1000];
    time_t joinDateTime;
};

void initBlockChain();
void deleteBlockChain();

void addUser(Person elem);
void inqure_bal(int user);
void inquire_transactions(int user);
void transact(int sender, int reciever, double amount);
void createBlock();
void attack();
void validate();

#endif