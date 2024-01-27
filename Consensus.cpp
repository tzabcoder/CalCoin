/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    Consensus.cpp 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Function contains the functions for the Consensus class functions. The functions 
    included are the class constructor and the choose mining node function to choose a 
    mining node from the pool of all mining nodes.
------------------------------------------------------------------------------------- */

#include "Consensus.h"

/*
FUNCTION: Consensus (constructor)
PARAMETERS:
	nothing
RETURNS:
	nothing

DESCRIPTION:
	Seeds the time for the random number generator.
*/
Consensus :: Consensus () {
    //Seed the time
    srand(time(NULL));
}

/*
FUNCTION: choose_mining_node
PARAMETERS:
	int pool_size:   number of nodes connected to the CalCoin network
RETURNS:
	int chosen_node:   the chosen node to mine the next block

DESCRIPTION:
	Randomly chooses the next node from all nodes that are connected 
    to the CalCoin network (via the random number generation function and 
    the seeded time from the constructor).
*/
int Consensus :: choose_mining_node (int pool_size) {
    int chosen_node = 0;

    chosen_node = rand() % pool_size;

    return chosen_node;
}