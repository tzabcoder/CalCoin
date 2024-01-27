/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    node.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file that cointains the function declarations and node constants for the
    network node software. It defines the node message headers and the functions to 
    run the node.
------------------------------------------------------------------------------------- */

#ifndef NODE_H
#define NODE_H

//Node Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "sha256.h"
#include "Blockchain.h"

using namespace std;

//Node Constant Variables
#define PORT_NUMBER 8080
#define BUFFERLEN 1024
#define NODE_CONNECTION "NODE"
#define REQ_WALLETADDR "WALLET_ADDR"
#define SUBSCRIBE "SUBSCRIBED"
#define CHOSEN "CHOSEN"
#define NOT_CHOSEN "NOT_CHOSEN"
#define TRANSACTION_END "END"
#define NEW_BLOCK "BLOCK"

//Node Function Declarations
string transaction_stream(vector<string>);
string stream_hash(string);
void* node_thread(void*);

#endif