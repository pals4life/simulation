//============================================================================
// @name        : Road.h
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description :
//============================================================================
#ifndef SIMULATION_ROAD_H
#define SIMULATION_ROAD_H

#include <deque>
#include <iterator>
#include <vector>
#include "Vehicles/IVehicle.h"
#include "TrafficSigns.h"

typedef std::deque<IVehicle*>::const_iterator Iter;

class Road
{

public:
	/**
     * REQUIRE(length > 0, "Failed to construct road: lenght must be greater than 0");
     * REQUIRE(!name.empty(), "Failed to construct road: name can not be empty");
     * REQUIRE(lanes > 0, "Failed to construct road: must at least have 1 lane");
     * REQUIRE(!zones.empty(), "Failed to construct road: must have at least 1 speed zone" );
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
	Road(const std::string& name, Road* const next, const double length, const uint32_t lanes, const std::vector<Zone>& zones, const std::vector<BusStop>& busStops, const std::vector<TrafficLight>& trafficLights);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling the destructor");
     */
    ~Road();

	bool properlyInitialized() const;

    //--------------------------------------------------------------------------------------------------//

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	void update();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling checkAndReset");
	 */
	bool checkAndReset();

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
     */
    void enqueue(IVehicle* vehicle);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling changeLaneIfPossible");
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling laneExists");
     * REQUIRE(index < fLanes[lane].size() and index >= 0, "Index is out of range");
     */
    void changeLaneIfPossible(IVehicle* vehicle, uint32_t lane, uint32_t index, bool left);

    //--------------------------------------------------------------------------------------------------//

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
	 */
	const Road* getNextRoad() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling setNextRoad");
	 */
	void setNextRoad(Road* fNextRoad);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getRoadLength");
	 */
	double getRoadLength() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getName");
	 */
	const std::string& getName() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNumLanes");
     */
    uint32_t getNumLanes() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling laneExists");
     */
    bool laneExists(uint32_t lane) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling operator[]");
     */
    const std::deque<IVehicle*>& operator[](uint32_t index) const;

    //--------------------------------------------------------------------------------------------------//
    //      al de onderstaande functies leiden tot een oneindige loop als banen een cirkel vormen       //
    //--------------------------------------------------------------------------------------------------//

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
	double getSpeedLimit(double position = 0) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextBusStop");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
    std::pair<BusStop*, double> getBusStop(double position = 0) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextTrafficLight");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
    std::pair<TrafficLight*, double> getTrafficLight(double position = 0) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextTrafficLight");
     * REQUIRE(laneExists(lane), "Cannot get vehicles on an non-existant lane");
     * REQUIRE(index < fLanes[lane].size() and index >= 0, "Index is out of range");
     */
    std::pair<const IVehicle*, double> getNextVehicle(uint32_t lane, uint32_t index) const;

    //--------------------------------------------------------------------------------------------------//

	/**
	 * REQUIRE(a->properlyInitialized(), "Road was not initialized when calling operator ==");
	 */
	friend bool operator==(Road* a, const std::string &b);

	/**
	 * REQUIRE(b->properlyInitialized(), "Road was not initialized when calling operator ==");
	 */
	friend bool operator==(const std::string &a, Road* b);

private:
    /**
     * ENSURE(fVehicles.front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
     */
    void dequeueFinishedVehicles();

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
     * REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
     * REQUIRE(lane < this->getNumLanes() and lane >= 0, "Cannot enqueue on an non-existant lane");
     *
     * ENSURE(getVehicles().front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
     */
    void enqueue(IVehicle* vehicle, uint32_t lane);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling dequeue");
     * REQUIRE(!isEmpty(), "Road cannot be empty when calling dequeue");
     * REQUIRE(lane < this->getNumLanes() and lane >= 0, "Cannot dequeue vehicle from an non-existant lane");
     */
    void dequeue(uint32_t lane);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
     */
    bool isEmpty() const;

    //--------------------------------------------------------------------------------------------------//

	double fRoadLength;
	std::string fName;

	Road* fNextRoad;
	std::vector< std::deque<IVehicle*> > fLanes;

	std::vector<Zone> fZones;
	std::vector<BusStop> fBusStops;
	std::vector<TrafficLight> fTrafficLights;

	Road* _initCheck;
};


#endif //SIMULATION_ROAD_H
