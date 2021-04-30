#include <stdio.h>
#include <stdlib.h>
#include "src/utils.h"

void main(){
    printf("You have started the blockchain.\n To make an action, chose the appropriate option\n");
    int input;
    while(1){
        printf("1 - Add a user\n");
        printf("2 - Make a transaction\n");
        printf("3 - Attack a random block\n");
        printf("4 - Validate the blockchain\n");
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

        if(input ==3){
            attack();
            continue;
        }

        if(input ==4){
            validate();
            continue;
        }
    }
    return;
}