/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Consensus.h 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Header file to contain the consensus model class definition. Consensus model 
    is used to choose the mining node from all connected nodes within the network.
    The consensus model only considers the current nodes connected to the CalCoin
    transaction pool.
------------------------------------------------------------------------------------- */

#ifndef CONSENSUS_H
#define CONSENSUS_H

#include <iostream>
#include <random>
#include <time.h>
#include "CVector.cpp"

using namespace std;

class Consensus {
    public: 
        Consensus();

        //Choose_Mining_Node (from transaction pool)
        int choose_mining_node(int);
};

#endif