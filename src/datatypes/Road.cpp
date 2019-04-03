//============================================================================
// @name        : Road.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================
#include <stdint.h>
#include "Road.h"
#include "../tests/DesignByContract.h"

Road::Road(const std::string& name, Road* next, double length, double speedLimit)
{

    REQUIRE(length > 0, "Failed to construct road: length must be greater than 0");
    REQUIRE(speedLimit > 0, "Failed to construct road: speed limit must be greater than 0");
    REQUIRE(!name.empty(), "Failed to construct road: name can not be empty");

    fName = name;
    fNextRoad = next;

    fRoadLength = length;
    fSpeedLimit = speedLimit;
    _initCheck = this;

    ENSURE(this->properlyInitialized(), "Road constructor must end in properlyInitialized state");
}

bool Road::properlyInitialized() const
{
    return _initCheck == this;
}

Road::~Road()
{
    for(uint32_t i = 0; i < fVehicles.size(); i++) delete fVehicles[i];
}

void Road::update()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");

    if(fVehicles.empty()) return;                           // if there are no vehicles we do not need to update.

    for(uint32_t i = 1; i < fVehicles.size(); i++)
    {
        fVehicles[i]->move(fVehicles[i-1], fSpeedLimit);
    }

    if(fNextRoad == NULL) fVehicles.front()->move(NULL, fSpeedLimit);
    else fVehicles.front()->move(fNextRoad->getBackVehicle(), fSpeedLimit, fRoadLength);

    while(!fVehicles.empty())                               // as long there are vehicles we can delete
    {
        if(fVehicles.front()->getPosition() > fRoadLength)  // check if they have left the road
        {
            dequeue();
        }
        else break;                                         // break, because if the first car is still on the road everyone behind him is also still on the road
    }
    if(fVehicles.empty()) return;
    ENSURE(fVehicles.front()->getPosition() <= fRoadLength, "Update failed to place vehicle on next road or delete it.");
}

bool Road::isDone()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isDone");
    for(uint32_t i = 0; i < fVehicles.size(); i++) fVehicles[i]->setMoved(false);

    return !isEmpty();
}

void Road::enqueue(Vehicle* const vehicle)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
    REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");

    fVehicles.push_back(vehicle);                       // we can add the new vehicle
    if(vehicle->getPosition() > fRoadLength) dequeue(); // immediately remove it when it has already traversed the whole road in one tick
}

void Road::dequeue()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling dequeue");
    REQUIRE(!isEmpty(), "Road was not initialized when calling dequeue");

    if(fNextRoad == NULL)
    {
         delete fVehicles.front();                                                      // free memory if they leave the simulation
    }
    else
    {
        fVehicles.front()->setPosition(fVehicles.front()->getPosition() - fRoadLength); // current position minus roadlength
        fNextRoad->enqueue(fVehicles.front());                                          // enqueue in next road if there is one
    }
    fVehicles.pop_front();                                                              // remove from the queue
}

bool Road::isEmpty() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
    return fVehicles.empty();
}

Vehicle* const Road::getBackVehicle() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBackVehicle");
    if(fVehicles.empty()) return NULL;
    return fVehicles.back();
}

Road* const Road::getNextRoad() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
    return fNextRoad;
}

double Road::getRoadLength() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getRoadLength");
    return fRoadLength;
}

double Road::getSpeedLimit() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
    return fSpeedLimit;
}

const std::string& Road::getName() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getName");
    return fName;
}

const std::deque<Vehicle*>& Road::getVehicles() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getVehicles");
    return fVehicles;
}

bool operator==(Road* const a, const std::string& b)
{
    REQUIRE(a->properlyInitialized(), "Road was not initialized when calling operator ==");
    return a->fName == b;
}

bool operator==(const std::string& a, Road* const b)
{
    REQUIRE(b->properlyInitialized(), "Road was not initialized when calling operator ==");
    return b->fName == a;
}

void Road::setNextRoad(Road* const kNextRoad)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling setNextRoad");
    Road::fNextRoad = kNextRoad;
}





