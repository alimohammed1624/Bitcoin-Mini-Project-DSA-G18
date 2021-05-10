#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include "src/hashtable.h"

// This file contains all functions related to block hash

// returns the original nonce of a block if it has been attacked
int getNonce(unsigned char *curHash, unsigned char *originalHash, int nonce)
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
void getBlockHash(unsigned char *hash, int blocknum, transaction *transactions, unsigned char *PrevBlockHash, int nonce)
{
    char tempBuffer[50 * num_t + 30];
    char transactionStr[50 * num_t];

    getTranactionStr(transactionStr, transactions);
    snprintf(tempBuffer, sizeof(tempBuffer), "%d, %s, %s", blocknum, PrevBlockHash, transactionStr);

    SHA1(tempBuffer, strlen((const char *)tempBuffer), hash);
    hash[1] ^= nonce;
    hash[0] ^= nonce >> 8;
}