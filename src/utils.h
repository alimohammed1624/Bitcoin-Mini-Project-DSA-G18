
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
    transaction *transactions;
    char joinDateTime[50];
};

Person addUser();
void transact(int sender, int reciever, double amount);
Block createBlock();
void attack();
void validate();