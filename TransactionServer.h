/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    TransactionServer.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file to define all constants and function prototypes for the 
    Transaction Pool (utility file). File also includes the necessary imports 
    for the main function and the utility functions.
------------------------------------------------------------------------------------- */

#ifndef TRANSACTIONSERVER_H
#define TRANSACTIONSERVER_H

//Local File Imports
#include "Consensus.h"
#include "Transaction.h"
#include "CVector.cpp"
#include "serialization.h"

//Server Imports
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
#include <poll.h>
#include <fstream>

using namespace std;

//Server Connection Variables
#define PORT 8080
#define MAX_CONNECTIONS 500
#define BUFFERLEN 1024
#define WALLET_LENGTH 32

#define ADDRESSBASE "database.txt"

//Transaction Headers
#define INVALID_RECEIVER_WALLET "INVALID_RECEIVER"
#define INVALID_SENDER_WALLET "INVALID_SENDER"
#define INVALID_TRANSACTION "INVALID_TRANSACTION"
#define VALID_TRANSACTION "VALID_TRANSACTION"

//Server Message Headers
#define CONNECT "ACCEPTED"
#define SUBSCRIBE "SUBSCRIBED"
#define SUB_VALIDATION "VALIDATED"
#define SUB_DENY "DENIED"
#define REQ_WALLETADDR "WALLET_ADDR"
#define REQUEST_SENDERADDR "SENDER_ADDRESS"
#define REQUEST_RECEIVERADDR "RECEIVER_ADDRESS"
#define REQUEST_AMOUNT "AMOUNT"
#define INVALID_REQUEST "INVALID"
#define INVALID_WALLET "INVALID_WALLET"
#define INVALID_AMOUNT "INVALID_AMOUNT"
#define LOG_WALLET "LOG_WALLET"
#define REQUEST_BALANCE "REQUEST_BALANCE"

//Mining Message Headers 
#define CHOSEN "CHOSEN"             //Notify the node that was chosen
#define NOT_CHOSEN "NOT_CHOSEN"     //Notify the nodes that were not chosen
#define END_STREAM "END"
#define NEW_BLOCK "BLOCK"

//Pool Constants
#define MAX_MINING_SIZE 5

//Transaction Server Function Prototypes
void mine_transaction_pool(Vector_Structure<string>&, Vector_Structure<string>&, Vector_Structure<string>&, Consensus, vector<int>&);
void process_node_connection(char*, int&, int&, Vector_Structure<string>&, Vector_Structure<string>&, Vector_Structure<string>&);
void log_wallet_address(int);
void retreive_wallet_balance(int);
bool process_transaction(char*, char*, int);
bool validate_IP_address(const string&);
bool validate_wallet_length(char*);
bool notify_network(vector<int>&, int, string, string);
char* send_transaction_data(Vector_Structure<string>&, int);
string process_client_connection(int&, int&, char*);

#endif