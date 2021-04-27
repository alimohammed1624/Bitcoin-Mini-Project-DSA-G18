#include <stdio.h>
#include <stdlib.h>
#include "src/utils.h"

//Global Variables for transasct
int send_recieve_arr[2][50];
double amount_arr[50];
int transaction_arr_ptr=0;

void transact(int sender, int reciever, double amount){
    if(amount == 0){
        printf("Amount can't be zero\n");
        return;
    }
    if(amount < 0){
        printf("amount can't be negative\n");
        return;
    }
    send_recieve_arr[0][transaction_arr_ptr]=sender;
    send_recieve_arr[1][transaction_arr_ptr]=reciever;
    amount_arr[transaction_arr_ptr]=amount;
    if(transaction_arr_ptr==50){
        createBlock();
        for(int i=0;i<50;i++){
            for(int j=0;j<2;j++){
                send_recieve_arr[j][i]=0;
            }
            amount_arr[i]=0;
        }
        transaction_arr_ptr=0;
    }
    else{
        transaction_arr_ptr+=1;
    }
    return;
}

Person addUser()
{
    Person p;
    printf("Date (dd/mm/yy) and time (hh:mm):");
    scanf("\n%[^\n]s", &p.joinDateTime);
    srand(time(0));
    p.uID = rand();
    printf("Your UPI id is %d", p.uID);
    p.balance = 0;
    p.numTransactions = 0;
    p.transactions = NULL;
    return p;
}
