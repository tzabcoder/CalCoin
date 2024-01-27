/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Transaction.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file to construct the transaction class. The transaction class is used to
    accemp transaction information, and create a 256 bit hash of the data.
------------------------------------------------------------------------------------- */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <sstream>
#include <time.h>
using namespace std;

class Transaction {
    public:
        //Constructor
        Transaction(string sAddr, string rAddr, int val);

        //Public Functions
        string GetHash();

    private:
        //Private Variables
        string _tHash;
        string _senderAddress;
        string _recieverAddress;
        time_t _tTime;
        int _value;
        string _status;

        void _Calculate_Hash();
};

#endif