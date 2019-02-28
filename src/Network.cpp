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

void Network::startSimulation()
{

    while(true)
    {
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