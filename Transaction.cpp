/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Transaction.cpp 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    File to contain all functions for the Transaction class. Functions create the 
    hash from the transaction information, in addition to having a function to retrieve 
    the hash from the class.
------------------------------------------------------------------------------------- */

#include "Transaction.h"
#include "sha256.h"

/*
FUNCTION: Transaction (constructor)
PARAMETERS:
	string sAddr:   string for the sender's wallet address
    string rAddr:   string for the receiver's wallet address
    int val:   the amount to send from the sender to the receiver
RETURNS:
	nothing

DESCRIPTION:
	Constructor sets the class variables, and sets the current time. After 
    setting the class variables, the constructor calculates the hash of all 
    class variables.
*/
Transaction :: Transaction (string sAddr, string rAddr, int val) {
    _senderAddress = sAddr;
    _recieverAddress = rAddr;
    _tTime = time(nullptr);
    _value = val;
    _status = "pending";

    _Calculate_Hash();
}

/*
FUNCTION: _Calculate_Hash()
PARAMETERS:
	nohting
RETURNS:
	nothing

DESCRIPTION:
	Creates a string stream from all class variables. Then uses the 
    SHA256 hashing function to create a 256 bit hash from the created string
    stream.
*/
void Transaction :: _Calculate_Hash () {
    stringstream ss;

    ss << _senderAddress << _recieverAddress << _tTime << _value << _status;
    
    //Create a SHA256 hash of the transaction
    _tHash = sha256(ss.str());
}

/*
FUNCTION: GetHash()
PARAMETERS:
	nothing
RETURNS:
	nothing

DESCRIPTION:
	Returns the hashed transaction string.
*/
string Transaction :: GetHash() {
    return _tHash;
}
