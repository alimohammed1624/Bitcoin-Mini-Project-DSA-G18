# DSA MiniProject - Group 18

DSA mini-project by Group 18 to simulate and understand the working of Bitcoin on a smaller scale

# Bitcoin User Instructions

<h2> Step 1- <strong>Compile and Run the code</strong></h2>
<ul>
<li> install the openssl library  <code>sudo apt-get install libssl-dev</code></li>
<li> cd into the directory and compile <code>gcc main.c inquire.c blockchain.c blockhash.c src/hashtable.c -lssl -lcrypto</code></li>
<li> run the code <code>/a.out</code></li>
</ul>
<h3><em>all these 3 commands are in bash file run.sh. So, running run.sh will be running all three steps at once</em></h3>

        ./run.sh 
or  

        sudo apt install libssl-dev
        gcc main.c inquire.c blockchain.c blockhash.c src/hashtable.c -lssl -lcrypto
        ./a.out

---

<h2>Step 2- <strong>Select the operation to perform from the displayed list (0-8)</strong></h2>

           LIST
            1 - Add a user
            2 - View balance
            3 - Make a transaction
            4 - Attack a random block
            5 - Validate the blockchain
            6 - View User's Transaction history
            7 - View Block chain
            8 - Display block by block number
            0 - Kill the blockchain

---

<h2>Step 3- <strong>Enter the required details for the respective funtion</strong></h2>
    
           LIST
            1 - Add a user - No details to be entered
            2 - View balance - Enter the Unique ID (uID)
            3 - Make a transaction - Enter the Unique ID (uID) of sender and reciever and amount to be transfered
            4 - Attack a random block - No details to be entered
            5 - Validate the blockchain - No details to be entered
            6 - View User Transaction history - Enter the Unique ID (uID)
            7 - View Block chain - No details to be entered
            8 - Display block by block number - Enter the block number of block you want to view
            0 - Kill the blockchain - No details to be entered

---
