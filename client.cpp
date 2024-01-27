#include "client.h"

Client :: Client() {}

//Connection Functions
void Client :: create_connection() {
    //Create Socket Stream
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT_NUMBER);

    connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    //Check for connection error
    if (connection_status < 0) {
        perror("ERROR: Could not connect to server...");
        exit(1);
    }
}

void Client :: close_connection() {
    //Close Socket Stream 
    close(network_socket);
}

void Client :: log_wallet_address(char* wallet) {
    create_connection();

    int numbytes;
    char buffer[BUFFERLEN];

    //Send log message 
    send(network_socket, LOG_WALLET_ADDRESS, strlen(LOG_WALLET_ADDRESS), 0);

    if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
        perror("ERROR: server receiving error...");
    }
    buffer[numbytes] = '\0';

    //Check Log Request
    if (strcmp(buffer, LOG_WALLET) == 0) {
        send(network_socket, wallet, strlen(wallet), 0);
    }

    close_connection();
}

void Client :: send_transaction(char* sender_addr, char* receiver_addr, char* transfer_amount) {
    create_connection();

    //Send data to socket -------------------------------------------------------------------
    send(network_socket, POST_TRANSACTION, strlen(POST_TRANSACTION), 0);

    if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: client receiving error...");
    }
    buffer[numbytes] = '\0';

    //Check for Sender message -------------------------------------------------------------------
    if (strcmp(buffer, REQUEST_SENDERADDR) == 0) {
        //Send Sender Address 
        if (send(network_socket, sender_addr, strlen(sender_addr), 0) == -1) {
            perror("ERROR: sender address");
        }
    }
    if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: client receiving error...");
    }
    buffer[numbytes] = '\0';

    if (strcmp(buffer, INVALID_WALLET) == 0) {
        cout << "Invalid sender address..." << endl;
    }
    else {
        cout << "Valid sender wallet address." << endl;

        //Check for Receiver message -------------------------------------------------------------------
        if (strcmp(buffer, REQUEST_RECEIVERADDR) == 0) {
            //Send Receiver Address
            if (send(network_socket, receiver_addr, strlen(receiver_addr), 0) == -1) {
                perror("ERROR: receiver address");
            }
        }
        if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
                perror("ERROR: client receiving error...");
        }
        buffer[numbytes] = '\0';

        if (strcmp(buffer, INVALID_WALLET) == 0) {
            cout << "Invalid receiver address..." << endl;
        }
        else {
            cout << "Valid receiver wallet address." << endl;

            //Check for Amount message -------------------------------------------------------------------
            if (strcmp(buffer, REQUEST_AMOUNT) == 0) {
                if (send(network_socket, transfer_amount, strlen(transfer_amount), 0) == -1) {
                    perror("ERROR: transfer amount...");
                }
            }
            //Listen For Transaction Status
            if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
                perror("ERROR: client receiving error...");
            }
            buffer[numbytes] = '\0';

            if (strcmp(buffer, INVALID_AMOUNT) == 0) {
                cout << "Invalid amount..." << endl;
            }
            else {
                cout << "Valid amount." << endl;
            }
        }
    }
    
    close_connection();
}

string Client :: request_balance(char* wallet_address) {
    int numbytes;
    char buffer[BUFFERLEN];

    create_connection();

    //Send Balance request header
    if (send(network_socket, REQUEST_BALANCE, strlen(REQUEST_BALANCE), 0) == -1) {
        cerr << "ERROR: could not request balance..." << endl;
    }
    if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: client receiving error...");
    }
    buffer[numbytes] = '\0';

    if (strcmp(buffer, WALLET_ADDR) == 0) {
        //Send the address
        send(network_socket, wallet_address, strlen(wallet_address), 0);

        if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
            perror("ERROR: client receiving error...");
        }
        buffer[numbytes] = '\0';
    }

    close_connection();

    return buffer;
}