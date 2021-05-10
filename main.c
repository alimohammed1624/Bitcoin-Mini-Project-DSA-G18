#include <stdio.h>
#include <stdlib.h>
#include "src/utils.h"

BlockChain B;

void inquire_transactions(int user);
void inqure_bal(int user);
void displayBlockChain();
void displayBlock(int bnum);

void main()
{
    srand(time(NULL));
    system("clear");

    initBlockChain();
    printf("You have started the blockchain.\nPress enter\n");
    scanf("[^n]");

    int input;
    while (1)
    {
        printf("\n\n");
        printf("1 - Add a user\n");
        printf("2 - View balance\n");
        printf("3 - Make a transaction\n");
        printf("4 - Attack a random block\n");
        printf("5 - Validate the blockchain\n");
        printf("6 - View User's Transaction history\n");
        printf("7 - View Block chain\n");
        printf("8 - Display block by block number\n");
        printf("0 - Kill the blockchain\n\n");
        printf("Enter your choice: ");
        scanf("%d", &input);

        if (input == 0)
        {
            deleteBlockChain();
            break;
        }

        else if (input == 1)
        {
            addUser();
        }

        else if (input == 2)
        {
            int user;
            printf("Enter user uID: ");
            scanf("%d", &user);
            inqure_bal(user);
        }

        else if (input == 3)
        {
            int sender;
            int reciever;
            double amount;
            printf("Enter sender ID: ");
            scanf("%d", &sender);
            printf("Enter reciever ID: ");
            scanf("%d", &reciever);
            printf("Enter amount to be transferred: ");
            scanf("%lf", &amount);
            transact(sender, reciever, amount);
        }

        else if (input == 4)
        {
            attack();
        }

        else if (input == 5)
        {
            validate();
        }

        else if (input == 6)
        {
            printf("Enter user ID: ");
            int user;
            scanf("%d", &user);
            inquire_transactions(user);
        }
        else if (input == 7)
        {
            displayBlockChain();
        }
        else if (input == 8)
        {
            int bnum;
            printf("Enter block number: ");
            scanf("%d", &bnum);
            displayBlock(bnum);
        }
        else
            printf("Invalid input! Try again!\n");
    }
}