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
#include <iostream>
#include "Network.h"
#include "../tests/DesignByContract.h"

const int Network::fgkMaxTicks = 1000;

Network::Network(const std::vector<Road*>& roads)
{
    fTicksPassed = 0;
    fRoads = roads;
    _initCheck = this;
    ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
}

Network::~Network()
{
    for(uint32_t i = 0; i < fRoads.size(); i++) delete fRoads[i];
}

bool Network::properlyInitialized() const
{
    return _initCheck == this;
}

int Network::getTicksPassed() const
{
    REQUIRE(this->properlyInitialized(), "Network was not initialized when calling getTicksPassed");
    return fTicksPassed;
}

void Network::startSimulation(int amountOfTicks)
{
    REQUIRE(this->properlyInitialized(), "Network was not initialized when calling startSimulation");
    REQUIRE(amountOfTicks >= 0, "Amount of ticks must be a positive integer");

    while(fTicksPassed < amountOfTicks)
    {
        bool simulationDone = true;

        for(uint32_t i = 0; i < fRoads.size(); i++)
        {
            if(fRoads[i]->update()) simulationDone = false;
        }
        if (simulationDone)
        {
            break;
        }

        fTicksPassed++;
        printNetwork();
    }
}

void Network::printNetwork()
{
    REQUIRE(this->properlyInitialized(), "Network was not initialized when calling printNetwork");

    std::ostringstream filename;
    filename << "outputfiles/output-" << fTicksPassed << ".txt";

    std::ofstream outputFile;
    outputFile.open(filename.str().c_str());
    std::cout << filename.str() << '\n';
    outputFile << "\nState of the network after " << fTicksPassed << " ticks have passed:\n\n";
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        outputFile << *fRoads[i];
    }
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        fRoads[i]->printVehicles(outputFile);
    }
    outputFile << std::flush;
    outputFile.close();
}