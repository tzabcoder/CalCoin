/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Blockchain.cpp 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    File contains all the functions to implement the blockchain class. Functions 
    are used to maintain the blockchain class.
------------------------------------------------------------------------------------- */

#include "BlockChain.h"

Blockchain::Blockchain() {
     block_count = 1;

     _vChain.__emplace_back(Block(0, "Genesis Block"));
     _nDifficulty = 5;
}

void Blockchain::AddBlock(Block bNew) {
     block_count += 1;

     bNew.sPrevHash = _GetLastBlock().GetHash();
     bNew.MineBlock(_nDifficulty);
     _vChain.push_back(bNew);
}

Block Blockchain::_GetLastBlock() const {
     return _vChain.back();
}

int Blockchain::get_block_count() {
     return block_count;
}

string Blockchain::get_mined_hash() {
     return _GetLastBlock().GetHash();
}