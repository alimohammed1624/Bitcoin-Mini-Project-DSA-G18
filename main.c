#include <stdio.h>
#include <stdlib.h>
#include "src/utils.h"

void main(){
    system("clear");
    printf("You have started the blockchain.\nPress enter\n");
    scanf("[^n]");
    int input;
    while(1){
        printf("\n\n\n");
        printf("1 - Add a user\n");
        printf("2 - View balance\n");
        printf("3 - Make a transaction\n");
        printf("4 - Attack a random block\n");
        printf("5 - Validate the blockchain\n");
        printf("6 - View Transaction history\n");
        printf("0 - Kill the blockchain\n");
        scanf("%d",&input);
        
        if(input == 0){
            break;
        }

        if(input ==1){
            addUser();
            continue;
        }

        if(input ==2){
            int user;
            printf("Enter user's uID\n");
            scanf("%d",&user);
            inqure_bal(user);
            continue;
        }

        if(input ==3){
            int sender;
            int reciever;
            double amount;
            printf("Enter sender ID\n");
            scanf("%d",&sender);
            printf("Enter reciever ID\n");
            scanf("%d",&reciever);
            printf("Enter amount to be sent\n");
            scanf("%lf",&amount);
            transact(sender,reciever,amount);
            continue;
        }

        if(input ==4){
            attack();
            continue;
        }

        if(input ==5){
            validate();
            continue;
        }

        if(input == 6){
            printf("Enter user's uID\n");
            int user;
            scanf("%d",&user);
            inquire_transactions(user);
            continue;
        }

    }
    return;
}