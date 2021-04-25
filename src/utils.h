
typedef struct block Block;
typedef struct t transaction;
typedef struct person Person;

struct t
{
    int SenderID;
    int RecieverID;
    int Amount;
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
    int balance;
    int numTransactions;
    transaction *transactions;
    char joinDateTime[50];
};
