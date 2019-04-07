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
#include <iostream>
#include "Road.h"
#include "DesignByContract.h"

Road::Road(const std::string& name, Road* const next, const double length, const double speedLimit, const uint32_t lanes)
{

    REQUIRE(length > 0, "Failed to construct road: length must be greater than 0");
    REQUIRE(speedLimit > 0, "Failed to construct road: speed limit must be greater than 0");
    REQUIRE(!name.empty(), "Failed to construct road: name can not be empty");
    REQUIRE(lanes > 0, "Failed to construct road: must at least have 1 lane");

    fName = name;
    fNextRoad = next;

    fRoadLength = length;
    fSpeedLimit = speedLimit;
    fLanes.resize(lanes);

    _initCheck = this;

    ENSURE(this->properlyInitialized(), "Road constructor must end in properlyInitialized state");
}

bool Road::properlyInitialized() const
{
    return _initCheck == this;
}

Road::~Road()
{
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(uint32_t j = 0; j < fLanes[i].size(); j++) delete fLanes[i][j];
    }
}

void Road::update()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");

    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        const std::deque<IVehicle*> vehicles = fLanes[i];
        if(vehicles.empty()) continue;

        for(uint32_t j = 1; j < vehicles.size(); j++)
        {
            const double pos = vehicles[j]->getPosition();
            std::pair<const IVehicle*, double> nextVehicle = std::pair<const IVehicle*, double>(vehicles[j-1], 0);
            vehicles[j]->move(nextVehicle, getNextTrafficLight(pos), getNextBusStop(pos), getSpeedLimit(pos));
        }

        const double    pos  = vehicles.front()->getPosition();
        const IVehicle* next = (fNextRoad == NULL) ? NULL : fNextRoad->getBackVehicle(i);
        std::pair<const IVehicle*, double> nextVehicle =  std::pair<const IVehicle*, double>(next, fRoadLength);
        vehicles.front()->move(nextVehicle, getNextTrafficLight(pos), getNextBusStop(pos), getSpeedLimit(pos));
    }

    dequeueFinishedVehicles();
}

void Road::dequeueFinishedVehicles()
{
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        while(!fLanes[i].empty())                                           // as long there are vehicles we can delete
        {
            if(fLanes[i].front()->getPosition() > fRoadLength) dequeue(i);  // check if they have left the road
            else break;                                                     // break, because if the first car is still on the road everyone behind him is also still on the road
        }
        if(fLanes[i].empty()) return;
        ENSURE(fLanes[i].front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
    }
}

bool Road::isDone()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isDone");
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(uint32_t j = 0; j < fLanes[i].size(); j++) fLanes[i][j]->setMoved(false);
    }

    return !isEmpty();
}

void Road::enqueue(IVehicle* const vehicle, const uint32_t lane)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
    REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
    REQUIRE(lane < this->getLanes(), "Cannot enqueue vehicle on an non-existant lane");

    fLanes[lane].push_back(vehicle);                        // we can add the new vehicle
    if(vehicle->getPosition() > fRoadLength) dequeue(lane); // immediately remove it when it has already traversed the whole road in one tick
}

void Road::dequeue(const uint32_t lane)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling dequeue");
    REQUIRE(!isEmpty(), "Road was not initialized when calling dequeue");
    REQUIRE(lane < this->getLanes(), "Cannot dequeue vehicle from an non-existant lane");
//    for(uint32_t i = 0; i < fLanes.size(); i++)
    if(fNextRoad == NULL)
    {
         delete fLanes[lane].front();                                                      // free memory if they leave the simulation
    }
    if(fNextRoad->getLanes() <= lane)
    {
        std::cerr << "Next road did not have enough lanes, removeing this vehicle from the simulation.\n";
        delete fLanes[lane].front();
    }
    else
    {
        fLanes[lane].front()->setPosition(fLanes[lane].front()->getPosition() - fRoadLength); // current position minus roadlength
        fNextRoad->enqueue(fLanes[lane].front(), lane);                                       // enqueue in next road if there is one
    }
    fLanes[lane].pop_front();                                                                // remove from the queue
}

bool Road::isEmpty() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
    for(uint32_t i = 0; i < fLanes.size(); i++)
        if(!fLanes[i].empty()) return false;
    return true;
}

IVehicle* const Road::getBackVehicle(const uint32_t lane) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBackVehicle");
    REQUIRE(lane < this->getLanes(), "Cannot get back vehicle on an non-existant lane");
    if(fLanes[lane].empty()) return NULL;
    return fLanes[lane].back();
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

uint32_t Road::getLanes() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getLanes");
    return fLanes.size();
}

const std::deque<IVehicle*>& Road::getVehicles(const uint32_t lane) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getVehicles");
    REQUIRE(lane < this->getLanes(), "Cannot get vehicles on an non-existant lane");
    return fLanes[lane];
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

double Road::getSpeedLimit(const double position) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
    REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
    return (--std::upper_bound(fZones.begin(), fZones.end(), position))->fSpeedlimit;
}

// als wegen een cirkel vormen en geen bushaltes bevatten, leidt dit tot een oneidige loop.
std::pair<const BusStop*, double> Road::getNextBusStop(const double position) const
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
std::pair<const TrafficLight*, double> Road::getNextTrafficLight(const double position) const
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





