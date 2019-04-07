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

class Road {

public:
	/**
     * REQUIRE(length > 0, "Failed to construct road: lenght must be greater than 0");
     * REQUIRE(speedLimit > 0, "Failed to construct road: speed limit must be greater than 0");
     * REQUIRE(!name.empty(), "Failed to construct road: name can not be empty");
     * REQUIRE(lanes > 0, "Failed to construct road: must at least have 1 lane");
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
	Road(const std::string &name, Road *next, double length, double speedLimit, uint32_t lanes);

	bool properlyInitialized() const;

	~Road();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	void update();

    /**
     * ENSURE(fVehicles.front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
     */
	void dequeueFinishedVehicles();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isDone");
	 */
	bool isDone();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
	 * REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
	 * REQUIRE(lane < this->getLanes(), "Cannot get back vehicle on an non-existant lane");
	 *
	 * ENSURE(getVehicles().front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
	 */
	void enqueue(IVehicle* vehicle, uint32_t lane);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 * REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
	 * REQUIRE(lane < this->getLanes(), "Cannot dequeue vehicle from an non-existant lane");
	 */
	void dequeue(uint32_t lane);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
	 */
	bool isEmpty() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBackVehicle");
	 * REQUIRE(lane < this->getLanes(), "Cannot get back vehicle on an non-existant lane");
	 */
    IVehicle* const getBackVehicle(uint32_t lane) const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
	 */
	Road *const getNextRoad() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling setNextRoad");
	 */
	void setNextRoad(Road* fNextRoad);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getRoadLength");
	 */
	double getRoadLength() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
	 * DEPRECATED!!!
	 */
	double getSpeedLimit() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getName");
	 */
	const std::string& getName() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getLanes");
     */
    uint32_t getLanes() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getVehicles");
	 * REQUIRE(lane < this->getLanes(), "Cannot get vehicles on an non-existant lane");
	 */
	const std::deque<IVehicle*>& getVehicles(uint32_t lane) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
	double getSpeedLimit(double position) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextBusStop");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
    std::pair<const BusStop*, double> getNextBusStop(double position) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextTrafficLight");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
    std::pair<const TrafficLight*, double> getNextTrafficLight(double position) const;

	/**
	 * REQUIRE(a->properlyInitialized(), "Road was not initialized when calling operator ==");
	 */
	friend bool operator==(Road* a, const std::string &b);

	/**
	 * REQUIRE(b->properlyInitialized(), "Road was not initialized when calling operator ==");
	 */
	friend bool operator==(const std::string &a, Road* b);

private:
	double fRoadLength;
	double fSpeedLimit;
	std::string fName;

	Road* fNextRoad;
	std::vector< std::deque<IVehicle*> > fLanes;

	std::vector<Zone> fZones;
	std::vector<BusStop> fBusStops;
	std::vector<TrafficLight> fTrafficLights;

	Road* _initCheck;
};


#endif //SIMULATION_ROAD_H
