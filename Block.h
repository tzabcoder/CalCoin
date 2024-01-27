/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Block.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file contains the class to implement the indivicual block for the 
    blockchain. The block class contains the functions and datafields for the 
    node to mine the block and add it to the blockchain.
------------------------------------------------------------------------------------- */

#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <time.h>

using namespace std;

/*
% Block Structure:
% Header:
% - Hash (previous block)
% - Timestamp 
% - Nonce 
% - Hash 
% Body:
% - Transaction List (Data)
*/

/*
     Class Block:
     public:
          Constructor Block(nIndexIn, const &sDataIn)
          - nIndexn => unsigned 32-bit integer
          - &sDataIn => constant string reference

          string sPrevHash
          - String variable containing the previous blocks' hash

          string GetHash()
          
          void MineBlock(nDifficulty)
          - nDifficulty => unsigned 32-bit integer

     private:
          uint32_t _nIndex => 32-bit unsigned integer
          int64_t _nNonce => 64-bit signed integer
          string _sData => string 
          string _sHash => string 
          time_t _tTime => type time

          string _CalculateHash() const
*/

class Block {
     public:
          //Constructor
          Block(uint32_t nIndexIn, const string &sDataIn);

          //Public Variables
          string sPrevHash;

          //Public Functions
          string GetHash();
          void MineBlock(uint32_t nDifficulty);

     private:
          //Private Variables
          uint32_t _nIndex;
          int64_t _nNonce;
          string _sData;
          string _sHash;
          time_t _tTime;

          //Private Functions
          string _CalculateHash() const; //const directive ensures function cannot change variables in block class
};

#endif
