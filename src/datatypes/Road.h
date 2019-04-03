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
#include "Vehicle.h"

class Road {

public:
	/**
     * REQUIRE(length > 0, "Failed to construct road: lenght must be greater than 0");
     * REQUIRE(speedLimit > 0, "Failed to construct road: speed limit must be greater than 0");
     * REQUIRE(!name.empty(), "Failed to construct road: name can not be empty");
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
	Road(const std::string &name, Road *next, double length, double speedLimit);

	bool properlyInitialized() const;

	~Road();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	void update();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isDone");
	 */
	bool isDone();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
	 *
	 * ENSURE(fVehicles.front()->getPosition() <= fRoadLength, "Update failed to place vehicle on next road or delete it.");
	 */
	void enqueue(Vehicle *const vehicle);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 * REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
	 */
	void dequeue();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
	 */
	bool isEmpty() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBackVehicle");
	 */
    Vehicle *const getBackVehicle() const;

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
	 */
	double getSpeedLimit() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getName");
	 */
	const std::string &getName() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getVehicles");
	 */
	const std::deque<Vehicle*>& getVehicles() const;

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
	std::deque<Vehicle*> fVehicles;

	Road* _initCheck;
};


#endif //SIMULATION_ROAD_H
