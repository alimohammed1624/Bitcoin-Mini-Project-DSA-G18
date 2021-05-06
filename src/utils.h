#ifndef UTILS_H_
#define UTILS_H_

typedef struct block Block;
typedef struct t transaction;
typedef struct person Person;

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
    char joinDateTime[50];
};

void addUser();
void inqure_bal(int user);
void inquire_transactions(int user);
void transact(int sender, int reciever, double amount);
void createBlock();
void attack();
void validate();

#endif