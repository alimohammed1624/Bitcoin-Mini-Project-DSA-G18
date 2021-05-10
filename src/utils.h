#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>

typedef struct block Block;
typedef struct blockchain BlockChain;
typedef struct t transaction;
typedef struct person Person;
typedef struct hashtable HashTable;
typedef struct node Node;

// macro for number of transactions in each block
#define num_t 50

// Global variable for blockchain
extern BlockChain B;

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
    unsigned char PrevBlockHash[30];
    int Nonce;
    Block *NextBlock;
};

struct person
{
    int uID;
    double balance;
    int numTransactions;
    transaction transactions[1000];
    struct tm joinDateTime;
};
struct hashtable
{
    int tsize;
    int num_elems;
    Node *table;
};

struct node
{
    Person user;
    Node *next;
};

struct blockchain
{
    Block *Bchain;                      // Pointer to first block in the chain
    unsigned char LastBlockHash[30];    // Stores the hash of the last block
    int numBlocks;                      // number of chains in the block
    HashTable userTable;                // hashtable of useres
    transaction transaction_arr[num_t]; // Transactions to be added to the next block
    int transaction_arr_ptr;            // number of transactions in transaction_arr
};

void initBlockChain();
void deleteBlockChain();

void addUser();
void inqure_bal(int user);
void inquire_transactions(int user);
void transact(int sender, int reciever, double amount);
void createBlock();
void attack();
void validate();

#endif