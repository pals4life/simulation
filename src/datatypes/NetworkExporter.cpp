//============================================================================
// @name        : NetworkExporter.cpp
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "NetworkExporter.h"
#include "../DesignByContract.h"
#include <stdlib.h>
#include <math.h>

NetworkExporter::NetworkExporter()
{
    _initCheck = this;
    fIsInitialized = false;
    ENSURE(this->properlyInitialized(), "constructor did not properlyInitialize");
}

void NetworkExporter::initialize(const Network* network, const std::string& kSimple, const std::string& kImpression)
{
    REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling printNetwork");
    REQUIRE(network->properlyInitialized(), "Network was not initialized when calling printNetwork");

    kfNetwork = network;
    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "could not make directory");
    fSimple.open(("outputfiles/" + kSimple + ".txt").c_str());
    ENSURE(fSimple.is_open(), "output file is not open");

    fImpression.open(("outputfiles/" + kImpression + ".txt").c_str());
    ENSURE(fImpression.is_open(), "output file is not open");

    double maxLength = 0;
    for(uint32_t i = 0; i < kfNetwork->fRoads.size(); i++)
    {
        const Road* road = kfNetwork->fRoads[i];
        fSimple << "Baan : " + road->getName() + '\n';
        fSimple << "  -> snelheidslimiet: "  << road->getSpeedLimit() * 3.6 << '\n';
        fSimple << "  -> lengte         : "  << road->getRoadLength()       << '\n';
        if (road->getRoadLength() > maxLength) maxLength = road->getRoadLength();
    }
    scale = maxLength / 120;
    fIsInitialized = true;
    addSection(0);
}

void NetworkExporter::finish()
{
    REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling finish");
    REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling finish");

    fSimple << std::flush;
    fSimple.close();
}

void NetworkExporter::addSection(uint32_t number)
{
    REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling addSection");
    REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling addSection");

    fSimple << "\n-------------------------------------------------\n";
    if(number != 1) fSimple << "State of the network after " << number << " ticks have passed:\n\n";
    else fSimple << "State of the network after " << number << " tick has passed:\n\n";
    for(uint32_t i = 0; i < kfNetwork->fRoads.size(); i++)
    {
        const Road* road = kfNetwork->fRoads[i];
        for(uint32_t j = 0; j < road->getNumLanes(); j++)
        {
            for(uint32_t k = 0; k < (*road)[j].size(); k++)
            {
                const IVehicle* vehicle = (*road)[j][k];
                fSimple << "Voertuig: " + vehicle->getType() +'('+ vehicle->getLicensePlate() + ")\n";
                fSimple << "  -> Baan    : " << road->getName()            << '\n';
                fSimple << "  -> Positie : " << vehicle->getPosition()     << '\n';
                fSimple << "  -> Snelheid: " << vehicle->getVelocity()*3.6 << '\n';
            }
        }
    }

    fImpression << "\n-------------------------------------------------\n";
    if (number != 1) fImpression << "State of the network after " << number << " ticks have passed:\n\n";
    else fImpression << "State of the network after " << number << " tick has passed:\n\n";
    for (uint32_t i = 0; i < kfNetwork->fRoads.size(); i++) {
        const Road *road = kfNetwork->fRoads[i];
        fImpression << road->getName() << "\t|\t";
        for (uint32_t j = 0; j < road->getNumLanes(); j++) {
            std::vector<std::vector<char> > lane;
            uint32_t max = 0;
            lane.resize(static_cast<int>(round(road->getRoadLength()/scale)));
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                uint32_t pos = static_cast<uint32_t >(round(vehicle->getPosition() / scale));
                lane[pos].push_back(toupper(vehicle->getType()[0]));
                if (lane[pos].size() > max) max = lane[pos].size();
            }
            for (uint32_t l = 0; l < max; ++l) {
                for (uint32_t k = 0; k < lane.size(); ++k) {
                    try {
                        fImpression << lane.at(k).at(l);
                    } catch (const std::out_of_range& oor) {
                        fImpression << (max == 0 ? '=' : ' ');
                    }
                }
                fImpression << '\n';
            }
        }
    }
}

bool NetworkExporter::properlyInitialized()
{
    return _initCheck == this;
}