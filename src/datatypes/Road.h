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
#include "IVehicle.h"

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
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 *
	 * ENSURE(fVehicles.front()->getPosition() <= fRoadLength, "Update failed to place vehicle on next road or delete it.");
	 */
	void enqueue(IVehicle *const vehicle);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 * REQUIRE(vehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
	 */
	void dequeue();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	bool isEmpty();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	IVehicle *const getBackVehicle() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	Road *const getNextRoad() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	void setNextRoad(Road *fNextRoad);

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	double getRoadLength() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	double getSpeedLimit() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	const std::string &getName() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling update");
	 */
	void printVehicles(std::ostream &stream) const;

	/**
	 * REQUIRE(a->properlyInitialized(), "Road was not initialized when calling operator ==");
	 */
	friend bool operator==(Road *const a, const std::string &b);

	/**
	 * REQUIRE(b->properlyInitialized(), "Road was not initialized when calling operator ==");
	 */
	friend bool operator==(const std::string &a, Road *const b);

	/**
	 * REQUIRE(road.properlyInitialized(), "Road was not initialized when calling operator >>");
	 */
	friend std::ostream &operator<<(std::ostream &stream, const Road &road);

private:
	double fRoadLength;
	double fSpeedLimit;
	std::string fName;

	Road* fNextRoad;
	std::deque<IVehicle*> fVehicles;

	Road* _initCheck;

};


#endif //SIMULATION_ROAD_H
