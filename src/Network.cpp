//============================================================================
// @name        : Network.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================
#include <stdint.h>
#include <sstream>
#include "Network.h"

const int Network::fgkMaxTicks = 100000;

Network::Network(const std::vector<Road*>& roads)
{
    fTicksPassed = 0;
    fRoads = roads;
}

Network::~Network()
{
    for(uint32_t i = 0; i < fRoads.size(); i++) delete fRoads[i];
}

int Network::getTicksPassed() const
{
    return fTicksPassed;
}

void Network::startSimulation(int amountOfTicks)
{
    printNetwork();
    while(fTicksPassed < amountOfTicks)
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

void Network::printNetwork()
{
    std::ostringstream filename;
    filename << "./outputfiles/output-" << fTicksPassed << ".txt";

    std::ofstream outputFile;
    outputFile.open(filename.str().c_str());
    outputFile << "\nState of the network after " << fTicksPassed << " ticks have passed:\n\n";
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        outputFile << fRoads[i];
    }
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        fRoads[i]->printVehicles(outputFile);
    }
}