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

Road::Road(const std::string& name, Road* const next, const double length, const uint32_t lanes, const std::vector<Zone>& zones, const std::vector<BusStop>& busStops, const std::vector<TrafficLight>& trafficLights)
{

    REQUIRE(length > 0    , "Failed to construct road: length must be greater than 0"   );
    REQUIRE(!name.empty() , "Failed to construct road: name can not be empty"           );
    REQUIRE(lanes > 0     , "Failed to construct road: must at least have 1 lane"       );
    REQUIRE(!zones.empty(), "Failed to construct road: must have at least 1 speed zone" );

    fName = name;
    fNextRoad = next;

    fRoadLength = length;
    fLanes.resize(lanes);

    fBusStops = busStops;
    fZones = zones;
    fTrafficLights = trafficLights;

    _initCheck = this;

    ENSURE(this->properlyInitialized(), "Road constructor must end in properlyInitialized state");
}

Road::~Road()
{
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(uint32_t j = 0; j < fLanes[i].size(); j++) delete fLanes[i][j];
    }
}

bool Road::properlyInitialized() const
{
    return _initCheck == this;
}


void Road::update()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");

    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        if(fLanes[i].empty()) continue;

        for(uint32_t j = 0; j < fLanes[i].size(); j++)
        {
            fLanes[i][j]->move(j,i, this);
        }
    }

    dequeueFinishedVehicles();
}

bool Road::checkAndReset()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling checkAndReset");
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(uint32_t j = 0; j < fLanes[i].size(); j++) fLanes[i][j]->setMoved(false);
    }
    return !isEmpty();
}

void Road::enqueue(IVehicle* const vehicle)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
    enqueue(vehicle, 0);
}

void changeLaneIfPossible(IVehicle* vehicle, uint32_t lane, uint32_t index, bool left)
{
    // TODO
}

const Road* Road::getNextRoad() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
    return fNextRoad;
}

double Road::getRoadLength() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getRoadLength");
    return fRoadLength;
}

const std::string& Road::getName() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getName");
    return fName;
}

uint32_t Road::getNumLanes() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNumLanes");
    return fLanes.size();
}

const std::deque<IVehicle*>& Road::operator[](const uint32_t index) const
{
    return fLanes[index];
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

//--------------------------------------------------------------------------------------------------//

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

//--------------------------------------------------------------------------------------------------//
//      al de onderstaande functies leiden tot een oneindige loop als banen een cirkel vormen       //
//--------------------------------------------------------------------------------------------------//

std::pair<const BusStop*, double> Road::getBusStop(const double position) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBusStop");
    REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
    const std::vector<BusStop>::const_iterator iter = std::upper_bound(fBusStops.begin(), fBusStops.end(), position);
    if(iter == fBusStops.end())
    {
        if(fNextRoad == NULL) return std::pair<const BusStop*, double>(NULL, 0);
        else
        {
            std::pair<const BusStop*, double> next = getBusStop(0);
            next.second += fRoadLength;
            return next;
        }
    }
    else return std::pair<const BusStop*, double>(iter.base(), 0);
}

std::pair<const TrafficLight*, double> Road::getTrafficLight(const double position) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getTrafficLight");
    REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
    const std::vector<TrafficLight>::const_iterator iter = std::upper_bound(fTrafficLights.begin(), fTrafficLights.end(), position);
    if(iter == fTrafficLights.end())
    {
        if(fNextRoad == NULL) return std::pair<const TrafficLight*, double>(NULL, 0);
        else
        {
            std::pair<const TrafficLight*, double> next = getTrafficLight(0);
            next.second += fRoadLength;
            return next;
        }
    }
    else return std::pair<const TrafficLight*, double>(iter.base(), 0);
}

std::pair<const IVehicle*, double> Road::getNextVehicle(uint32_t lane, uint32_t index) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getTrafficLight");
    REQUIRE(lane < this->getNumLanes() and lane >= 0, "Cannot get vehicles on an non-existant lane");
    REQUIRE(index < fLanes[lane].size() and index >= 0, "Index is out of range");

    if(index == 0)
    {
        const Road* iter = fNextRoad;
        double offset = fRoadLength;
        while(iter != NULL)
        {
            if(iter->getNumLanes() <= lane) break;
            else if(iter->fLanes[lane].empty())
            {
                iter = iter->fNextRoad;
                offset += iter->fRoadLength;
            }
            else return std::pair<const IVehicle*, double>(iter->fLanes[lane].back(), offset);
        }
        return std::pair<const IVehicle*, double>(NULL, 0);
    }
    else return std::pair<const IVehicle*, double>(fLanes[lane][index-1], 0);
}

//--------------------------------------------------------------------------------------------------//

void Road::dequeueFinishedVehicles()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");

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

void Road::enqueue(IVehicle* const vehicle, const uint32_t lane)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
    REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
    REQUIRE(lane < this->getNumLanes() and lane >= 0, "Cannot enqueue on an non-existant lane");

    fLanes[lane].push_back(vehicle);                        // we can add the new vehicle
    if(vehicle->getPosition() > fRoadLength) dequeue(lane); // immediately remove it when it has already traversed the whole road in one tick
}

void Road::dequeue(const uint32_t lane)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling dequeue");
    REQUIRE(!isEmpty(), "Road was not initialized when calling dequeue");
    REQUIRE(lane < this->getNumLanes() and lane >= 0, "Cannot dequeue vehicle from an non-existant lane");

    if(fNextRoad == NULL)
    {
        delete fLanes[lane].front();                                                      // free memory if they leave the simulation
    }
    if(fNextRoad->getNumLanes() <= lane)
    {
        std::cerr << "Next road did not have enough lanes, removeing this vehicle from the simulation.\n";
        delete fLanes[lane].front();
    }
    else
    {
        fLanes[lane].front()->setPosition(fLanes[lane].front()->getPosition() - fRoadLength); // current position minus roadlength
        fNextRoad->enqueue(fLanes[lane].front(), lane);                                       // enqueue in next road if there is one
    }
    fLanes[lane].pop_front();                                                                 // remove from the queue
}

bool Road::isEmpty() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
    for(uint32_t i = 0; i < fLanes.size(); i++)
        if(!fLanes[i].empty()) return false;
    return true;
}





