#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include "Colors.h"
#include "client.h"

using namespace std;

//Utility Constants 
#define WALLET_LENGTH 32
#define WALLET_FILE "wallet_address1.txt"

//Utility Functions
void view_wallet_balance(Client);
string process_wallet_address(Client);
string generate_wallet_address();
string get_wallet_addr();
bool send_coins(Client);

//UI Functions
void calcoin_header();
int menu();

#endif