#include <stdio.h>
#include <stdlib.h>
#include "src/utils.h"

//Global Variables for transasct
int send_recieve_arr[2][50];
double amount_arr[50];
int transaction_arr_ptr=0;

void transact(int sender, int reciever, double amount){
    if(amount <= 0){
        printf("Amount can't be zero or negative");
        return;
    }
    send_recieve_arr[0][transaction_arr_ptr]=sender;
    send_recieve_arr[1][transaction_arr_ptr]=reciever;
    amount_arr[transaction_arr_ptr]=amount;
    if(transaction_arr_ptr==50){
        createBlock();
        transaction_arr_ptr=0;
    }
    else{
        transaction_arr_ptr+=1;
    }
}