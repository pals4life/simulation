//============================================================================
// @name        : Network.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================
#include <stdint.h>
#include "Network.h"

Network::Network(const std::vector<Road*>& roads)
{
    fTicksPassed = 0;
    fRoads = roads;
}


int Network::getTicksPassed() const
{
    return fTicksPassed;
}

void Network::startSimulation(int amountOfTicks)
{

    while(fTicksPassed < amountOfTicks)
    {
        printNetwork();
        fTicksPassed++;
        bool simulationDone = true;
        for(uint32_t i = 0; i < fRoads.size(); i++)
        {
            fRoads[i]->update();
            if (!fRoads[i]->isEmpty())
            {
                simulationDone = false;
            }
        }
        if (simulationDone)
        {
            break;
        }
    }
}

void Network::printNetwork() {
    std::ofstream outputFile;
    outputFile.open("./outputfiles/output.txt");
    outputFile << "\nState of the network after " << fTicksPassed << " ticks have passed:\n";
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        outputFile << fRoads[i];
    }
}