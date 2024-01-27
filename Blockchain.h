/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Blockchain.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file contains the class to implement and maintain the blockchain structure
    run on the network nodes.
------------------------------------------------------------------------------------- */

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <cstdlib>
#include <vector>
#include "Block.h"

using namespace std;

class Blockchain {
public:
     //Constructor
     Blockchain();

     //Public Functions
     void AddBlock(Block bNew);
     int get_block_count();
     string get_mined_hash();

private:
     int block_count;

     //Private Variables
     uint32_t _nDifficulty;
     vector<Block> _vChain;

     //Private Functions
     Block _GetLastBlock() const; //const directive ensures function cannot change variables in Blockchain class
};

#endif

