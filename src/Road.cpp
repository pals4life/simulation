//============================================================================
// @name        : Road.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     :
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================
#include <stdint.h>
#include "Road.h"


Road::Road(const std::string& name, Road* next, double length, double speedLimit)
{
    fName = name;
    fNextRoad = next;

    fRoadLength = length;
    fSpeedLimit = speedLimit;
}

Road::~Road()
{
    for(uint32_t i = 0; i < fVehicles.size(); i++) delete fVehicles[i];
}

void Road::update()
{
    if(fVehicles.empty()) return;                       // if there are no vehicles we do not need to update.

    for(uint32_t i = 1; i < fVehicles.size(); i++)
    {
        fVehicles[i]->move(fVehicles[i-1], fSpeedLimit);
    }

    if(fNextRoad == NULL) fVehicles.front()->move(NULL, fSpeedLimit);
    else fVehicles.front()->move(fNextRoad->getBackVehicle(), fSpeedLimit);

    while(!fVehicles.empty())                           // as long there are vehicles we can delete
    {
        if(fVehicles[0]->getPosition() > fRoadLength)   // check if they have left the road
        {
            if(fNextRoad == NULL) delete fVehicles[0];  // free memory if they leave the simulation
            else fNextRoad->enqueue(fVehicles[0]);      // enqueue in next road if there is one
            fVehicles.pop_front();                      // remove from the queue
        }
        else break;                                     // break, because if the first car is still on the road everyone behind him is also still on the road
    }
}

void Road::enqueue(IVehicle* const vehicle)
{
    fVehicles.push_back(vehicle);
}

void Road::dequeue()
{
    fVehicles.pop_front();
}

bool Road::isEmpty()
{
    return fVehicles.empty();
}

IVehicle* const Road::getBackVehicle() const
{
    if(fVehicles.empty()) return NULL;
    return fVehicles.back();
}



Road* const Road::getNextRoad() const
{
    return fNextRoad;
}

double Road::getRoadLength() const
{
    return fRoadLength;
}

double Road::getSpeedLimit() const
{
    return fSpeedLimit;
}

const std::string& Road::getName() const
{
    return fName;
}

void Road::printVehicles(std::ostream& stream) const
{
    for(uint32_t i = 0; i < fVehicles.size(); i++)
    {
        fVehicles[i]->printVehicle(stream, fName);
    }
}

bool operator==(Road* const a, const std::string& b)
{
    return a->fName == b;
}

bool operator==(const std::string& a, Road* const b)
{
    return b->fName == a;
}

std::ostream& operator<<(std::ostream& stream, const Road& road)
{
    stream << "Baan : " + road.fName + '\n';
    stream << "  -> snelheidslimiet: "  << road.fSpeedLimit << '\n';
    stream << "  -> lengte         : "  << road.fRoadLength << '\n';
    return stream;
}

void Road::setNextRoad(Road *fNextRoad) {
    Road::fNextRoad = fNextRoad;
}





