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
#include "../exporters/NetworkExporter.h"
#include "../DesignByContract.h"
#include "../exporters/VehicleExporter.h"

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

void Network::startSimulation(const Window* window, bool print, bool gui)
{
    REQUIRE(this->properlyInitialized(), "Network was not initialized when calling startSimulation");
    if(print)
    {
        VehicleExporter::init("statistics");
        NetworkExporter::init(this, "simple", "impression");
    }

    while(fTicksPassed < fgkMaxTicks)
    {
        if(not gui or checkWindow(window->getState()))
        {
            if(update()) break;

            if(print) NetworkExporter::addSection(this, fTicksPassed);
            if(gui  ) Window::processEvents();
        }
    }

    if(print)
    {
        VehicleExporter::finish();
        NetworkExporter::finish();
    }

    std::cout << "the simulation has ended after " << fTicksPassed << " ticks\n";
}

bool Network::update()
{
    bool simulationDone = true;

    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        fRoads[i]->updateTrafficSigns();
    }
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        fRoads[i]->updateVehicles();
    }
    for(uint32_t i = 0; i < fRoads.size(); i++)
    {
        if(fRoads[i]->checkAndReset()) simulationDone = false;
    }

    fTicksPassed++;
    return simulationDone;
}

bool Network::checkWindow(Window::EState state) const
{
    switch(state)
    {
        case Window::kPlay:
            return true;
        case Window::kNext:
            return true;
        case Window::kPause:
            Window::delay(500);
            return false;
        default:
            Window::delay(500);
            return false;
    }

}

const std::vector<Road *> &Network::getRoads() const {
    return fRoads;
}
