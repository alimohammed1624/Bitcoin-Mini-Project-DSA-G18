sudo apt install libssl-dev
gcc main.c inquire.c blockchain.c blockhash.c src/hashtable.c -lssl -lcrypto
./a.out
