#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/utils.h"

//Global Variables for transasct
transaction transaction_arr[50];
int transaction_arr_ptr = 0;

// Blockchain
Block *BlockChain;

//Global Variables for User
Person user_arr[1000];

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
    transaction_arr[transaction_arr_ptr].SenderID = sender;
    transaction_arr[transaction_arr_ptr].RecieverID = reciever;
    transaction_arr[transaction_arr_ptr].Amount = amount;
    user_arr[sender].transactions[user_arr[sender].numTransactions].SenderID = sender;
    user_arr[sender].transactions[user_arr[sender].numTransactions].RecieverID = reciever;
    user_arr[sender].transactions[user_arr[sender].numTransactions].Amount = amount;
    user_arr[sender].balance -= amount;
    user_arr[reciever].transactions[user_arr[reciever].numTransactions].SenderID = sender;
    user_arr[reciever].transactions[user_arr[reciever].numTransactions].RecieverID = reciever;
    user_arr[reciever].transactions[user_arr[reciever].numTransactions].Amount = amount;
    user_arr[reciever].balance += amount;
    user_arr[sender].numTransactions += 1;
    user_arr[reciever].numTransactions += 1;
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

void addUser()
{
    int r = 0;

    srand(time(0));
    while (1)
    {
        r = rand() % 1000;
        if (user_arr[r].joinDateTime[0] == 0 && user_arr[r].joinDateTime[1] == 0)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    user_arr[r].uID = r;
    printf("Date (dd/mm/yy) and time (hh:mm):\n");
    scanf("\n%[^\n]s", user_arr[r].joinDateTime);
    printf("Your uID is %d\n", user_arr[r].uID);
    user_arr[r].balance = 1000;
    user_arr[r].numTransactions = 0;
    return;
}

void createBlock()
{
    return;
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
    }
    printf("Attack failed: Block with number %d does not exist\n", randNum);
    return;
}

void validate()
{
    return;
}