/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    VerifyTransaction.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file to define all constants and function prototypes for the 
    transaction verification module. The functions verify the sender wallet 
    address, verify the sender amount, and transfer coins from the sender 
    wallet address to the receiver address.
------------------------------------------------------------------------------------- */

#ifndef VERIFYTRANSACTION_H
#define VERIFYTRANSACTION_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>

using namespace std;

#define FILENAME "database.txt"

bool verify_wallet_address(char*, int);
bool verify_sender_amount(char*, int, int);
void transfer_value(int, char*, char*);

#endif