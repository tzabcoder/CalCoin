/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    node.cpp
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    File contains all functions to make the netowrk nodes run effectively
------------------------------------------------------------------------------------- */

#include "node.h"

string transaction_stream(vector<string> v) {
    stringstream ss;

    //Populate the string stream
    copy(v.begin(), v.end(), ostream_iterator<string>(ss, "\n"));

    return ss.str();
}

string stream_hash(string stream) {
    string stream_hash;

    stream_hash = sha256(stream);

    return stream_hash;
}

void* node_thread(void* args) {
    char* ip_addr = "127.0.0.1";
    char* wallet_addr = "DKSJFH689KJHhkjhdsfkjh876UHGDFLPQ8";

    int network_socket;
    int connection_status;
    struct sockaddr_in server_address;

    //Receiving Buffer 
    int numbytes;
    char buffer[BUFFERLEN];
    vector<string> t_pool;
    bool mining_node = false;

    //Blockchain Variables
    int block_count; 
    string block_hash;
    string t_stream;
    Blockchain block_chain = Blockchain();

    //Create Socket Stream
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT_NUMBER);

    connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (connection_status < 0) {
        perror("ERROR: Could not connect to server...");
        return 0;
    }

    //Send data to socket 
    send(network_socket, NODE_CONNECTION, strlen(NODE_CONNECTION), 0);

    cout << "Node connection established with server..." << endl;

    if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: node receiving error...");
    }
    buffer[numbytes] = '\0';

    //Check IP Address Request -----------------------------------------------------------
    if (strcmp(buffer, SUBSCRIBE) == 0) {
        cout << "Node: Sending IP address..." << endl;

        send(network_socket, ip_addr, strlen(ip_addr), 0);
    }

    if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: node receiving error...");
    }
    buffer[numbytes] = '\0';

    //Check Node Request -----------------------------------------------------------
    if (strcmp(buffer, REQ_WALLETADDR) == 0) {
        cout << "Node: Sending wallet address..." << endl;

        send(network_socket, wallet_addr, strlen(wallet_addr), 0);
    }

    while (1) {
        //Process node commands
        if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
			cerr << "ERROR: could not accept client response..." << endl;
		}
		buffer[numbytes] = '\0';

        if (strcmp(buffer, CHOSEN) == 0) {
            //Mining Process
            cout << "Node Mining..." << endl;
            mining_node = true;
        }
        else if (strcmp(buffer, NOT_CHOSEN) == 0) {
            //Node Not Chosen for mining
            cout << "Node Not Chosen for mining..." << endl;
            mining_node = false;
            t_pool.clear();

            if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
                cerr << "ERROR: could not accept client response..." << endl;
            }
            buffer[numbytes] = '\0';

            cout << "Mining Node Wallet Address: " << buffer << endl;
        }
        else if (strcmp(buffer, NEW_BLOCK) == 0) {
            if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
                cerr << "ERROR: could not accept client response..." << endl;
            }
            buffer[numbytes] = '\0';

            cout << "New block: " << buffer << endl;
            //Add block to blockchain
        }
        else {
            if (strcmp(buffer, TRANSACTION_END) != 0) {
                cout << "Transaction Hash: " << buffer << endl;
                t_pool.push_back(buffer);
            }
        }

        //Check if the mining node
        if (mining_node == true) {
            if (strcmp(buffer, TRANSACTION_END) == 0) {
                t_stream = transaction_stream(t_pool);
                t_stream = stream_hash(t_stream);

                cout << endl;
                cout << "Transaction Stream Hash: " << t_stream << endl;

                block_count = block_chain.get_block_count();         //Get number of blocks 
                block_chain.AddBlock(Block(block_count, t_stream));  //Add block to blockchain
                block_hash = block_chain.get_mined_hash();           //Get hash of mined block

                //Send Block Hash back to the transaction server
                int s_len = block_hash.length();
                char b_hash[s_len + 1];
                strcpy(b_hash, block_hash.c_str());

                cout << "Sending Data to Transaction Server..." << endl;

                if (send(network_socket, b_hash, sizeof(b_hash), 0) == -1){
                    cout << "Could not send transaction hash header to server..." << endl;
                }
            }
        }
    }

    //Close Connection
    close(network_socket);
    pthread_exit(NULL);

    return 0;
}