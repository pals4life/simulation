//============================================================================
// @name        : Road.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include "Road.h"
#include "../DesignByContract.h"
#include "util.h"

Road::Road(const std::string& kName, Road* const kNext, const double kLength, const uint32_t kLanes, const std::vector<const Zone*>& kZones, const std::vector<const BusStop*>& kBusStops, const std::vector<const TrafficLight*>& kTrafficLights)
{
    REQUIRE(kLength > 0    , "Failed to construct road: length must be greater than 0"   );
    REQUIRE(!kName.empty() , "Failed to construct road: name can not be empty"           );
    REQUIRE(kLanes < 100 and kLanes > 0, "Failed to construct road: must at least have 1 lane or less than 100");
    REQUIRE(!kZones.empty(), "Failed to construct road: must have at least 1 speed zone" );
    REQUIRE(kZones[0]->getPosition() == 0, "Must have a zone at position 0"              );

    fName = kName;
    fNextRoad = kNext;

    fRoadLength = kLength;
    fLanes.resize(kLanes);

    fBusStops = kBusStops;
    fZones = kZones;
    fTrafficLights = kTrafficLights;

    fMergingVehicles = {};

    _initCheck = this;

    ENSURE(this->properlyInitialized(), "Road constructor must end in properlyInitialized state");
}

Road::~Road()
{
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(Iter iter = fLanes[i].begin(); iter < fLanes[i].end(); ++iter) delete *iter;
    }
}

bool Road::properlyInitialized() const
{
    return _initCheck == this;
}

void Road::updateTrafficSigns()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling updateTrafficSigns");
    for(uint32_t i = 0; i < fTrafficLights.size(); i++) fTrafficLights[i]->update();
    for(uint32_t i = 0; i < fBusStops     .size(); i++) fBusStops     [i]->update();
}

void Road::updateVehicles()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling updateVehicles");

    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(uint32_t j = 0; j < fLanes[i].size(); j++)
        {
            fLanes[i][j]->move(i,j, this);
        }
    }

    for(uint32_t i = 0; i < fMergingVehicles.size();)
    {
        uint32_t& timer = std::get<0>(fMergingVehicles[i]);
        if(++timer > 5)
        {
            const uint32_t kLane = std::get<1>(fMergingVehicles[i]);
            Iter iter = std::find(begin(fLanes[kLane]), end(fLanes[kLane]), std::get<2>(fMergingVehicles[i]));
            (*iter)->setMerging(false);
            fLanes[kLane].erase(iter);
            fMergingVehicles.erase(fMergingVehicles.begin() + i);
        }
        else i++;
    }

    dequeueFinishedVehicles();
}

bool Road::checkAndReset()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling checkAndReset");
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        for(Iter iter = fLanes[i].begin(); iter < fLanes[i].end(); ++iter) (*iter)->setMoved(false);
    }
    return !isEmpty();
}

void Road::enqueue(IVehicle* const kVehicle)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
    enqueue(kVehicle, 0);
}

bool Road::changeLaneIfPossible(IVehicle* vehicle, const uint32_t kLane, const bool kLeft)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling changeLaneIfPossible");
    REQUIRE(laneExists(kLane + (kLeft ? 1 : -1)), "Cannot go to non-existing lane");

    std::vector<IVehicle*>& newLane = fLanes[kLane + (kLeft ? 1 : -1)];
    const double ideal = 1.5 * vehicle->getVelocity();

    // this isn't specified but vehicles are not allowed to switch lanes when entering or leaving a road.
    if(vehicle->getPosition() < ideal or vehicle->getPosition() > fRoadLength - ideal) return false;

    if(!newLane.empty())
    {
        uint32_t infront = -1;
        uint32_t behind = -1;
        for(uint32_t i = 0; i < newLane.size(); i++)
        {
            if(comparePositionGT<IVehicle>(vehicle->getPosition(), newLane[i]))
            {
                behind = i;
                if(i != 0) infront = i-1;
                break;
            }
        }
        if(behind == uint32_t(-1)) infront = newLane.size() - 1;

        if(behind  != uint32_t(-1) and newLane[behind ]->getPosition() + ideal > vehicle->getPosition()) return false;
        if(infront != uint32_t(-1) and newLane[infront]->getPosition() - ideal < vehicle->getPosition()) return false;

        if(behind == uint32_t(-1)) newLane.push_back(vehicle);
        else newLane.insert(begin(newLane)+behind, vehicle);

        fMergingVehicles.push_back(std::tuple<uint32_t, uint32_t, const IVehicle*>(0, kLane, vehicle));
    }
    else
    {
        // if the lane is empty we can easily push it back
        newLane.insert(newLane.begin(), vehicle);
        fMergingVehicles.push_back(std::tuple<uint32_t, uint32_t, const IVehicle*>(0, kLane, vehicle));
    }
    return true;
}

void Road::updateNextVehicles()
{
    if(fNextRoad != NULL) fNextRoad->updateVehicles();
}

bool Road::isEmpty() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        if(!fLanes[i].empty()) return false;
    }
    return true;
}

const Road* Road::getNextRoad() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
    return fNextRoad;
}

Road* Road::getNextRoad()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
    return fNextRoad;
}

void Road::setNextRoad(Road* const kNextRoad)
{
    REQUIRE(this->properlyInitialized()     , "Road was not initialized when calling setNextRoad");
    REQUIRE(kNextRoad == NULL or kNextRoad->properlyInitialized(), "kNextRoad was not initialized when calling setNextRoad");
    Road::fNextRoad = kNextRoad;
    ENSURE(getNextRoad() == kNextRoad, "new next road not set when calling setNextRoad");
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

bool Road::laneExists(uint32_t kLane) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling laneExists");
    return kLane < getNumLanes();
}

const std::vector<IVehicle*>& Road::operator[](const uint32_t kIndex) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling operator[]");
    REQUIRE(laneExists(kIndex), "lane does not exist");
    return fLanes[kIndex];
}

//--------------------------------------------------------------------------------------------------//
//              al de onderstaande functies zijn voor verkeerstekens methodes                       //
//--------------------------------------------------------------------------------------------------//

void Road::addZone(const Zone* kZone)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling addZone");
    REQUIRE(kZone->properlyInitialized(), "Zone was not properly initialized");

    insert_sorted<Zone>(fZones, kZone);
}

void Road::addBusStop(const BusStop* kBusStop)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling addBusStop");
    REQUIRE(kBusStop->properlyInitialized(), "BusStop was not properly initialized");

    insert_sorted<BusStop>(fBusStops, kBusStop);
}

void Road::addTrafficLight(const TrafficLight* kTrafficLight)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling addTrafficLight");
    REQUIRE(kTrafficLight->properlyInitialized(), "TrafficLight was not properly initialized");

    insert_sorted<TrafficLight>(fTrafficLights, kTrafficLight);
}

std::vector<const Zone*> Road::getZones() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getZones");
    return fZones;
}

std::vector<const BusStop*> Road::getBusStops() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBusStops");
    return fBusStops;
}

std::vector<const TrafficLight*> Road::getTrafficLights() const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getTrafficLights");
    return fTrafficLights;
}

//--------------------------------------------------------------------------------------------------//
//      al de onderstaande functies leiden tot een oneindige loop als banen een cirkel vormen       //
//--------------------------------------------------------------------------------------------------//

double Road::getSpeedLimit(const double kPosition) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
    REQUIRE(kPosition >= 0 and kPosition < getRoadLength(), "position not valid");
    return (*--std::upper_bound(fZones.begin(), fZones.end(), kPosition, comparePosition<Zone>))->getSpeedlimit();
}

std::pair<const BusStop*, double> Road::getBusStop(const double kPosition) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBusStop");
    REQUIRE(kPosition >= 0 and kPosition < getRoadLength(), "position not valid");

    const Road* current = this;
    double offset = 0;

    while(current != NULL)
    {
        const std::vector<const BusStop*>::const_iterator kIter = std::upper_bound(fBusStops.begin(), fBusStops.end(), kPosition, comparePosition<BusStop>);
        if(kIter == fBusStops.end())
        {
            if(fNextRoad == NULL) break;
            offset += current->fRoadLength;
            current = current->fNextRoad;
        }
        else return std::pair<const BusStop*, double>(*kIter, offset);
    }
    return std::pair<const BusStop*, double>(NULL, 0);
}

std::pair<const TrafficLight*, double> Road::getTrafficLight(const double kPosition) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getTrafficLight");
    REQUIRE(kPosition >= 0 and kPosition < getRoadLength(), "position not valid");

    const Road* current = this;
    double offset = 0;

    while(current != NULL)
    {
        const std::vector<const TrafficLight*>::const_iterator kIter = std::upper_bound(fTrafficLights.begin(), fTrafficLights.end(), kPosition, comparePosition<TrafficLight>);
        if(kIter == fTrafficLights.end())
        {
            if(fNextRoad == NULL) break;
            offset += current->fRoadLength;
            current = current->fNextRoad;
        }
        else return std::pair<const TrafficLight*, double>(*kIter, offset);
    }
    return std::pair<const TrafficLight*, double>(NULL, 0);
}

std::pair<const IVehicle*, double> Road::getNextVehicle(const uint32_t kLane, const uint32_t kIndex) const
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getTrafficLight");
    REQUIRE(laneExists(kLane), "Cannot get vehicles on an non-existant kLane");
    REQUIRE(kIndex < fLanes[kLane].size(), "Index is out of range");

    if(kIndex == 0)
    {
        const Road* iter = fNextRoad;
        double offset = fRoadLength;
        while(iter != NULL)
        {
            if(iter->getNumLanes() <= kLane) break;
            else if(iter->fLanes[kLane].empty())
            {
                offset += iter->fRoadLength;
                iter = iter->fNextRoad;
            }
            else return std::pair<const IVehicle*, double>(iter->fLanes[kLane].back(), offset);
        }
        return std::pair<const IVehicle*, double>(NULL, 0);
    }
    else return std::pair<const IVehicle*, double>(fLanes[kLane][kIndex-1], 0);
}

//--------------------------------------------------------------------------------------------------//

void Road::dequeueFinishedVehicles()
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling updateVehicles");

    for(uint32_t i = 0; i < fLanes.size(); i++)
    {
        while(!fLanes[i].empty())                                           // as long there are vehicles we can delete
        {
            if(fLanes[i].front()->getPosition() >= fRoadLength) dequeue(i); // check if they have left the road
            else break;                                                     // break, because if the first car is still on the road everyone behind him is also still on the road
        }

        if(fLanes[i].empty()) continue;
        ENSURE(fLanes[i].front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
    }
}

void Road::enqueue(IVehicle* const kVehicle, const uint32_t kLane)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
    REQUIRE(kVehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
    REQUIRE(kLane < this->getNumLanes(), "Cannot enqueue on an non-existant lane");

    fLanes[kLane].push_back(kVehicle);                        // we can add the new kVehicle
    if(kVehicle->getPosition() > fRoadLength) dequeue(kLane); // immediately remove it when it has already traversed the whole road in one tick
}

void Road::dequeue(const uint32_t kLane)
{
    REQUIRE(this->properlyInitialized(), "Road was not initialized when calling dequeue");
    REQUIRE(!isEmpty(), "Road cannot be empty when calling dequeue");
    REQUIRE(laneExists(kLane), "Cannot dequeue vehicle from an non-existant lane");

    if(fNextRoad == NULL)
    {
        delete fLanes[kLane].front();                                                      // free memory if they leave the simulation
    }
    else if(fNextRoad->getNumLanes() <= kLane)
    {
        std::cerr << "Next road did not have enough lanes, removing this vehicle from the simulation.\n";
        delete fLanes[kLane].front();
    }
    else
    {
        fLanes[kLane].front()->setPosition(fLanes[kLane].front()->getPosition() - fRoadLength); // current position minus roadlength
        fNextRoad->enqueue(fLanes[kLane].front(), kLane);                                       // enqueue in next road if there is one
    }
    fLanes[kLane].erase(fLanes[kLane].begin());                                                 // remove from the queue
}





