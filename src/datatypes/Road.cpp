//============================================================================
// @name        : Road.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================
#include <stdint.h>
#include <algorithm>
#include "Road.h"
#include "DesignByContract.h"

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
    if(fVehicles.empty()) return;   // if there are no vehicles we do not need to update.

    for(uint32_t i = 1; i < fVehicles.size(); i++)
    {
        const double pos = fVehicles[i]->getPosition();
        std::pair<const IVehicle*, double> nextVehicle = std::pair<const IVehicle*, double>(fVehicles[i-1], 0);
        fVehicles[i]->move(nextVehicle, getNextTrafficLight(pos), getNextBusStop(pos), getSpeedLimit(pos));
    }

    const double    pos  = fVehicles.front()->getPosition();
    const IVehicle* next = (fNextRoad == NULL) ? NULL : fNextRoad->getBackVehicle();
    std::pair<const IVehicle*, double> nextVehicle =  std::pair<const IVehicle*, double>(next, fRoadLength);
    fVehicles.front()->move(nextVehicle, getNextTrafficLight(pos), getNextBusStop(pos), getSpeedLimit(pos));

    dequeueFinishedVehicles();
}

void Road::dequeueFinishedVehicles()
{
    while(!fVehicles.empty())                                           // as long there are vehicles we can delete
    {
        if(fVehicles.front()->getPosition() > fRoadLength) dequeue();   // check if they have left the road
        else break;                                                     // break, because if the first car is still on the road everyone behind him is also still on the road
    }
    if(fVehicles.empty()) return;
    ENSURE(fVehicles.front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
}

bool Road::isDone()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isDone");
    for(uint32_t i = 0; i < fVehicles.size(); i++) fVehicles[i]->setMoved(false);

    return !isEmpty();
}

void Road::enqueue(IVehicle* const vehicle)
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

IVehicle* const Road::getBackVehicle() const
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

const std::deque<IVehicle*>& Road::getVehicles() const
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

double Road::getSpeedLimit(double position) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
    REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
    return (--std::upper_bound(fZones.begin(), fZones.end(), position))->fSpeedlimit;
}

// als wegen een cirkel vormen en geen bushaltes bevatten, leidt dit tot een oneidige loop.
std::pair<const BusStop*, double> Road::getNextBusStop(double position) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextBusStop");
    REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
    const std::vector<BusStop>::const_iterator iter = std::upper_bound(fBusStops.begin(), fBusStops.end(), position);
    if(iter == fBusStops.end())
    {
        if(fNextRoad == NULL) return std::pair<const BusStop*, double>(NULL, 0);
        else
        {
            std::pair<const BusStop*, double> next = getNextBusStop(0);
            next.second += fRoadLength;
            return next;
        }
    }
    else return std::pair<const BusStop*, double>(iter.base(), 0);
}

// als wegen een cirkel vormen en geen bushaltes bevatten, leidt dit tot een oneidige loop.
std::pair<const TrafficLight*, double> Road::getNextTrafficLight(double position) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextTrafficLight");
    REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
    const std::vector<TrafficLight>::const_iterator iter = std::upper_bound(fTrafficLights.begin(), fTrafficLights.end(), position);
    if(iter == fTrafficLights.end())
    {
        if(fNextRoad == NULL) return std::pair<const TrafficLight*, double>(NULL, 0);
        else
        {
            std::pair<const TrafficLight*, double> next = getNextTrafficLight(0);
            next.second += fRoadLength;
            return next;
        }
    }
    else return std::pair<const TrafficLight*, double>(iter.base(), 0);
}





