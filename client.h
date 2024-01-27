#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

using namespace std;

//Client Constants
#define PORT_NUMBER 8080
#define BUFFERLEN 1024
//Sending Message Headers
#define POST_TRANSACTION "POST"
#define LOG_WALLET_ADDRESS "LOG"
#define REQUEST_BALANCE "REQUEST_BALANCE"

//Receiving Message Headers
#define REQUEST_SENDERADDR "SENDER_ADDRESS"
#define REQUEST_RECEIVERADDR "RECEIVER_ADDRESS"
#define REQUEST_AMOUNT "AMOUNT"
#define LOG_WALLET "LOG_WALLET"
#define WALLET_ADDR "WALLET_ADDR"

//Server Validation Messages 
#define INVALID_WALLET "INVALID_WALLET"
#define INVALID_AMOUNT "INVALID_AMOUNT"
#define VALID_TRANSACTION "VALID_TRANSACTION"
#define INVALID_TRANSACTION "INVALID_TRANSACTION"

//Process Client Connection
class Client{
    public:
        Client();

        //Public Functions
        void send_transaction(char*, char*, char*);
        void log_wallet_address(char*);
        string request_balance(char*);

    private:
        //Private Functions
        void create_connection();
        void close_connection();

        //Private Variables
        int network_socket;
        int connection_status;
        struct sockaddr_in server_address;
        int numbytes;
        char buffer[BUFFERLEN];
};

#endif