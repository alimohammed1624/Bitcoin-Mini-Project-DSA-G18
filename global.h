#include "src/hashtable.h"
#include "src/utils.h"

// Global variables
transaction transaction_arr[50];
int transaction_arr_ptr = 0;
Block *BlockChain;
HashTable userTable;
