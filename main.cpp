/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    main.cpp
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Main function for the transaction server listens for connections from the end-users
    and the nodes to connect to the CalCoin network. The connections can either POST a 
    transaction to the transaction server, or connect as a NODE. If the message header 
    is neither POST or NODE, it is an invalid connection. 
------------------------------------------------------------------------------------- */

#include "TransactionServer.h"

/*
FUNCTION: main
PARAMETERS:
	int socket_fd:   Socket descriptor of the the client 
RETURNS:
	nothing

DESCRIPTION:
	Main function of the transaction server. Listens for connections on the network
    and calls the proper functions to process the message requests.
*/
int main() {
    //_____________________Transaction Pool Variables_____________________
    //Declare Variables
    int serverSocket;
    int newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    //Socket Array (Vector)
    vector<int> node_sockets;

    //Structure Variables
    Vector_Structure<string> ip_addr_list;
    Vector_Structure<string> stake_list;
    Vector_Structure<string> wallet_list;
    int struct_size = 0;

    //Consensus Model 
    Consensus c_model = Consensus();

    //Transaction Pool Variables
    Vector_Structure<string> t_pool;
    string transaction_hash;
    int curr_t_size = 0;

    //Initialize Socket Variables
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    //Bind Socket to Address and Port Number
    if (bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("ERROR: binding error...");
        exit(1);
    }

    //Listen on Socket 
    if (listen(serverSocket, 50) == 0) {
        cout << "Listening..." << endl;
    }
    else {
        perror("ERROR: listening on socket...");
        exit(1);
    }

    //Receiving Buffer
    int numbytes;
    char buffer[BUFFERLEN];

    //Runs Transaction Server
    while (1) {
        addr_size = sizeof(serverStorage);

        //Extract first connection in the queue
        newSocket = accept(serverSocket, (struct sockaddr*) &serverStorage, &addr_size);
        if ((numbytes = recv(newSocket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: server receiving error...");
        }
        buffer[numbytes] = '\0';

        //Client Connection
        if (strcmp(buffer, "POST") == 0) {
            transaction_hash = process_client_connection(newSocket, numbytes, buffer);

            if (transaction_hash.length() > 0) {
                t_pool.push(transaction_hash);

                //Check if new 50 of transactions
                curr_t_size = t_pool.size();
                if (curr_t_size % MAX_MINING_SIZE == 0) {
                    mine_transaction_pool(t_pool, wallet_list, ip_addr_list, c_model, node_sockets);
                    
                    for (int i = 0; i < curr_t_size; i++) {
                        t_pool.pop();
                    }
                }
            }
        }
        //Node Connection
        else if (strcmp(buffer, "NODE") == 0) {
            //Process Node Connection - add to sockets array
            process_node_connection(buffer, newSocket, numbytes, ip_addr_list, wallet_list, stake_list);
            node_sockets.push_back(newSocket);
        }
        //Log Client Wallet Address to File
        else if (strcmp(buffer, "LOG") == 0) {
            log_wallet_address(newSocket);
        }
        //Request Balance
        else if (strcmp(buffer, REQUEST_BALANCE) == 0) {
            retreive_wallet_balance(newSocket);
        }
        //Invalid Connection 
        else {
            cout << "Invalid connection... " << endl;
        }
    }

    return 0;
}