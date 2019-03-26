//============================================================================
// @name        : NetworkExporter.cpp
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <libnet.h>
#include "NetworkExporter.h"
#include "../tests/DesignByContract.h"

NetworkExporter::NetworkExporter()
{
    _initCheck = this;
    fIsInitialized = false;
    ENSURE(this->properlyInitialized(), "constructor did not properlyInitialize");
}

void NetworkExporter::initialize(const Network* network, const std::string& name)
{
    REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling printNetwork");
    REQUIRE(network->properlyInitialized(), "Network was not initialized when calling printNetwork");

    kfNetwork = network;
    mkdir("outputfiles", 0777);
    fOutputFile.open(("outputfiles/" + name + ".txt").c_str());
    ENSURE(fOutputFile.is_open(), "output file is not open");

    for(uint32_t i = 0; i < kfNetwork->fRoads.size(); i++)
    {
        fOutputFile << *kfNetwork->fRoads[i];
    }
    fIsInitialized = true;
}

void NetworkExporter::finish()
{
    REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling finish");
    REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling finish");

    fOutputFile << std::flush;
    fOutputFile.close();
}

void NetworkExporter::addSection(uint32_t number)
{
    REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling addSection");
    REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling addSection");

    fOutputFile << "\n-------------------------------------------------\n";
    if(number > 1) fOutputFile << "State of the network after " << number << " ticks have passed:\n\n";
    else fOutputFile << "State of the network after " << number << " tick has passed:\n\n";
    for(uint32_t i = 0; i < kfNetwork->fRoads.size(); i++)
    {
        kfNetwork->fRoads[i]->printVehicles(fOutputFile);
    }
}

bool NetworkExporter::properlyInitialized()
{
    return _initCheck == this;
}