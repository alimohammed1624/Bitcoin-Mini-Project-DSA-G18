#ifndef UTILS_H_
#define UTILS_H_

#include "hashtable.h"

typedef struct block Block;
typedef struct t transaction;
typedef struct person Person;

// Global variables for transasct
extern transaction transaction_arr[50];
extern int transaction_arr_ptr = 0;

// Blockchain
extern Block *BlockChain;

// Global variable for user hashtable
extern HashTable usertable;

struct t
{
    int SenderID;
    int RecieverID;
    double Amount;
};

struct block
{
    int BlockNumber;
    transaction Transactions[50];
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