/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    VerifyTransaction.cpp
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    File cointains the functions listed in the VerificationTransaction header 
    listing file. This module cointains the functions to verify the wallet address
    sent the transaction server, verify the sender amount recieved by the 
    transaction server and transfer coins from the sender to the receiver wallet 
    address.
------------------------------------------------------------------------------------- */

#include "VerifyTransaction.h"

/*
FUNCTION: verify_wallet_address
PARAMETERS:
	char* wallet_addr:   wallet address to check if it exists
    int wallet_len:   byte length for the wallet address
RETURNS:
	boolean match:   match status (if the wallet exists)

DESCRIPTION:
	Opens the database file on the transaction server to check if the 
    wallet address exists in the database. The function then uses the match
    variable to return the verification status.
*/
bool verify_wallet_address(char* wallet_addr, int wallet_len) {
    string temp;
    string address;
    bool match = false;
    ifstream f_ptr;

    f_ptr.open(FILENAME);
    if (f_ptr.is_open()) {
        while (getline(f_ptr, temp)) {
            address = temp.substr(0, wallet_len);

            int l = address.length();
            char w[l + 1];
            strcpy(w, address.c_str());

            if (strcmp(w, wallet_addr) == 0) {
                match = true;
                break;
            }
        }
    }
    f_ptr.close();

    return match;
}

/*
FUNCTION: verify_sender_amount
PARAMETERS:
	char* wallet_addr:   sender wallet address 
    int wallet_len:   number of bytes for the sender wallet address
    int amount:   amount of coins to send from sender to receiver wallet address
RETURNS:
	boolean match:   match status (if the number of coins exist in the sender wallet)

DESCRIPTION:
	Opens the database file on the transaction server to check if the 
    wallet address exists in the database and verifys that there is enough
    coins in the sender wallet address
*/
bool verify_sender_amount(char* wallet_addr, int amount, int wallet_len) {
    string temp;
    string address;
    string temp_amount;
    int log_length;
    bool enough_coins = false;
    ifstream f_ptr;

    f_ptr.open(FILENAME);
    if (f_ptr.is_open()) {
        while (getline(f_ptr, temp)) {
            log_length = temp.length();
            address = temp.substr(0, wallet_len);

            int l = address.length();
            char w[l + 1];
            strcpy(w, address.c_str());

            if (strcmp(w, wallet_addr) == 0) {
                //Check the amount
                temp_amount = temp.substr(wallet_len+3, log_length);

                //Check wallet amount >= test amount
                l = temp_amount.length();
                char a[l + 1];
                strcpy(a, temp_amount.c_str());

                int wallet_amount = atoi(a);

                if (wallet_amount >= amount) {
                    enough_coins = true;
                }
                else {
                    enough_coins = false;
                }
                break;
            }
        }
    }
    f_ptr.close();

    return enough_coins;
}

/*
FUNCTION: transfer_value
PARAMETERS:
	int amount:   amount to send from one wallet address to the other
    char* sender_addr:   wallet address of the sender
    char* receiver_addr:   wallet address of the receiver
RETURNS:
	nothing

DESCRIPTION:
	Opens the database file on the server, and sends the coins from the 
    sender wallet address to the other. The wallet addresses and sending 
    amount are verified before the transaction server transfers CalCoins.
*/
void transfer_value(int amount, char* sender_addr, char* receiver_addr) {
    char* TEMP_FILENAME = "temp_base.txt";
    string temp;
    string address;
    string temp_amount;
    int log_length;
    int wallet_len = 32;
    bool enough_coins = false;
    ifstream f_ptr;
    ofstream temp_file;

    f_ptr.open(FILENAME);
    if (f_ptr.is_open()) {
        //Create the temp file
        temp_file.open(TEMP_FILENAME);

        while (getline(f_ptr, temp)) {
            log_length = temp.length();
            address = temp.substr(0, wallet_len);

            int l = address.length();
            char w[l + 1];
            strcpy(w, address.c_str());

            if (strcmp(w, sender_addr) == 0) {
                //Check the amount
                temp_amount = temp.substr(wallet_len+3, log_length);

                //Check wallet amount >= test amount
                l = temp_amount.length();
                char a[l + 1];
                strcpy(a, temp_amount.c_str());

                //Update Sender amount
                int wallet_amount = atoi(a);
                wallet_amount = wallet_amount - amount;

                temp_file << address << " - " << wallet_amount << endl;
            }
            else if (strcmp(w, receiver_addr) == 0) {
                //Check the amount
                temp_amount = temp.substr(wallet_len+3, log_length);

                //Check wallet amount >= test amount
                l = temp_amount.length();
                char a[l + 1];
                strcpy(a, temp_amount.c_str());

                //Update Sender amount
                int wallet_amount = atoi(a);
                wallet_amount = wallet_amount + amount;

                temp_file << address << " - " << wallet_amount << endl;
            }
            else {
                temp_file << temp << endl;
            }
        }

        f_ptr.close();
        if (temp_file.is_open()) {
            temp_file.close();
        }

        remove(FILENAME);
        rename(TEMP_FILENAME, FILENAME);
    }
}