//============================================================================
// @name        : Road.h
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================
#ifndef SIMULATION_ROAD_H
#define SIMULATION_ROAD_H

#include <deque>
#include <iterator>
#include <vector>
#include "vehicles/IVehicle.h"
#include "TrafficSigns.h"

typedef std::vector<IVehicle*>::const_iterator Iter;

class Road
{

public:
	/**
     * REQUIRE(length > 0, "Failed to construct road: lenght must be greater than 0"       );
     * REQUIRE(!name.empty(), "Failed to construct road: name can not be empty"            );
     * REQUIRE(lanes > 0, "Failed to construct road: must at least have 1 lane or less than 100");
     * REQUIRE(!zones.empty(), "Failed to construct road: must have at least 1 speed zone" );
     * REQUIRE(kZones[0]->getPosition() == 0, "Must have a zone at position 0"             );
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
    Road(const std::string& kName, Road* kNext, double kLength, uint32_t kLanes, const std::vector<const Zone*>& kZones, const std::vector<const BusStop*>& kBusStops, const std::vector<const TrafficLight*>& kTrafficLights);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling the destructor");
     */
    ~Road();

	bool properlyInitialized() const;

    //--------------------------------------------------------------------------------------------------//

    /**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling updateTrafficSigns");
	 */
    void updateTrafficSigns();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling updateVehicles");
	 */
	void updateVehicles();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling checkAndReset");
	 */
	bool checkAndReset();

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
     */
    void enqueue(IVehicle* kVehicle);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling changeLaneIfPossible");
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling laneExists");
     * REQUIRE(index < fLanes[lane].size(), "Index is out of range");
     */
    bool changeLaneIfPossible(IVehicle* vehicle, uint32_t kLane, uint32_t kIndex, bool kLeft);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextTrafficLight");
     * REQUIRE(laneExists(lane), "Cannot get vehicles on an non-existant lane");
     * REQUIRE(index < fLanes[lane].size(), "Index is out of range");
     *
     * ENSURE(getNextVehicle(kLane, kIndex).getMoved());
     */
    void updateNextVehicles();

    //--------------------------------------------------------------------------------------------------//

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling isEmpty");
     */
    bool isEmpty() const;

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
	 */
	const Road* getNextRoad() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextRoad");
     */
    Road* getNextRoad();

	/**
	 * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling setNextRoad");
	 * REQUIRE(kNextRoad == NULL or kNextRoad->properlyInitialized(), "kNextRoad was not initialized when calling setNextRoad");
	 * ENSURE(getNextRoad() == kNextRoad, "new next road not set when calling setNextRoad");
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
    bool laneExists(uint32_t kLane) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling operator[]");
     * REQUIRE(laneExists(kIndex), "lane does not exist");
     */
    const std::vector<IVehicle*>& operator[](uint32_t kIndex) const;

    //--------------------------------------------------------------------------------------------------//
    //              al de onderstaande functies zijn voor verkeerstekens methodes                       //
    //--------------------------------------------------------------------------------------------------//

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling addZone");
     * REQUIRE(kZone->properlyInitialized(), "Zone was not properly initialized");
     */
    void addZone(const Zone* kZone);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling addBusStop");
     * REQUIRE(kBusStop->properlyInitialized(), "BusStop was not properly initialized");
     */
    void addBusStop(const BusStop* kBusStop);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling addTrafficLight");
     * REQUIRE(kTrafficLight->properlyInitialized(), "TrafficLight was not properly initialized");
     */
    void addTrafficLight(const TrafficLight* kTrafficLight);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getZones");
     */
    std::vector<const Zone*> getZones() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getBusStops");
     */
    std::vector<const BusStop*> getBusStops() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getTrafficLights");
     */
    std::vector<const TrafficLight*> getTrafficLights() const;

    //--------------------------------------------------------------------------------------------------//
    //      al de onderstaande functies leiden tot een oneindige loop als banen een cirkel vormen       //
    //--------------------------------------------------------------------------------------------------//

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getSpeedLimit");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
	double getSpeedLimit(double kPosition = 0) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextBusStop");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
    std::pair<const BusStop*, double> getBusStop(double kPosition = 0) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextTrafficLight");
     * REQUIRE(position >= 0 and position < getRoadLength(), "position not valid");
     */
    std::pair<const TrafficLight*, double> getTrafficLight(double kPosition = 0) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling getNextVehicle");
     * REQUIRE(laneExists(lane), "Cannot get vehicles on an non-existant lane");
     * REQUIRE(index < fLanes[lane].size(), "Index is out of range");
     */
    std::pair<const IVehicle*, double> getNextVehicle(uint32_t kLane, uint32_t kIndex) const;

private:
    /**
     * ENSURE(fVehicles.front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
     */
    void dequeueFinishedVehicles();

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling enqueue");
     * REQUIRE(kVehicle->properlyInitialized(), "Vehicle was not initialized when calling enqueue");
     * REQUIRE(kLane < this->getNumLanes(), "Cannot enqueue on an non-existant lane");
     *
     * ENSURE(getVehicles().front()->getPosition() <= getRoadLength(), "Update failed to place vehicle on next road or delete it.");
     */
    void enqueue(IVehicle* kVehicle, uint32_t kLane);

    /**
     * REQUIRE(this->properlyInitialized(), "Road was not initialized when calling dequeue");
     * REQUIRE(!isEmpty(), "Road cannot be empty when calling dequeue");
     * REQUIRE(kLane < this->getNumLanes() and kLane >= 0, "Cannot dequeue vehicle from an non-existant lane");
     */
    void dequeue(uint32_t kLane);

    //--------------------------------------------------------------------------------------------------//

	double fRoadLength;
	std::string fName;

	Road* fNextRoad;
	std::vector<std::vector<IVehicle*> > fLanes;
	std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> fMergingVehicles;

	std::vector<const Zone*> fZones;
	std::vector<const BusStop*> fBusStops;
	std::vector<const TrafficLight*> fTrafficLights;

	Road* _initCheck;
};


#endif //SIMULATION_ROAD_H
