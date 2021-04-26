#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/utils.h"

void main(){
    while(1){
        char input[10];
        scanf("%s",input);
        
        if(!strcmp(input,"exit")){
            break;
        }

        if(!strcmp(input,"add")){
            addUser();
            continue;
        }

        if(!strcmp(input,"transact")){
            int sender;
            int reciever;
            double amount;
            transact(sender,reciever,amount);
            continue;
        }

        if(!strcmp(input,"attack")){
            attack();
            continue;
        }

        if(!strcmp(input,"validate")){
            validate();
            continue;
        }
    }
    return;
}