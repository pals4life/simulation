//============================================================================
// @name        : NetworkExporter.cpp
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "NetworkExporter.h"
#include "DesignByContract.h"
#include <stdlib.h>

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
    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "could not make directory");
    fOutputFile.open(("outputfiles/" + name + ".txt").c_str());
    ENSURE(fOutputFile.is_open(), "output file is not open");

    for(uint32_t i = 0; i < kfNetwork->fRoads.size(); i++)
    {
        const Road* road = kfNetwork->fRoads[i];
        fOutputFile << "Baan : " + road->getName() + '\n';
        fOutputFile << "  -> snelheidslimiet: "  << road->getSpeedLimit() * 3.6 << '\n';
        fOutputFile << "  -> lengte         : "  << road->getRoadLength()       << '\n';
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
        const Road* road = kfNetwork->fRoads[i];
        for(uint32_t j = 0; j < road->getNumLanes(); j++)
        {
            for(uint32_t k = 0; k < (*road)[j].size(); k++)
            {
                const IVehicle* vehicle = (*road)[j][k];
                fOutputFile << "Voertuig: " + vehicle->getType() +'('+ vehicle->getLicensePlate() + ")\n";
                fOutputFile << "  -> Baan    : " << road->getName()            << '\n';
                fOutputFile << "  -> Positie : " << vehicle->getPosition()     << '\n';
                fOutputFile << "  -> Snelheid: " << vehicle->getVelocity()*3.6 << '\n';
            }
        }
    }
}

bool NetworkExporter::properlyInitialized()
{
    return _initCheck == this;
}