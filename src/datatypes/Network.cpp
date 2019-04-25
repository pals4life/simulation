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
#include "NetworkExporter.h"
#include "../DesignByContract.h"

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

void Network::startSimulation(const Window& window, bool print, bool gui)
{
    REQUIRE(this->properlyInitialized(), "Network was not initialized when calling startSimulation");

    NetworkExporter exporter;

    if(print) exporter.initialize(this, "test");

    while(fTicksPassed < fgkMaxTicks)
    {
        if(!gui or checkWindow(window)) update();
        if(print) exporter.addSection(fTicksPassed);
        if(gui) Window::processEvents();
    }
    if(print) exporter.finish();
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

bool Network::checkWindow(const Window& window) const
{
    switch(window.getState())
    {
        case Window::play:
            return true;
        case Window::next:
            return true;
        case Window::pause:
            Window::delay(500);
            return false;
        default:
            Window::delay(500);
            return false;
    }

}