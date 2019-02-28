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

class Road
{

public:
    Road(const std::string& name, Road* next, double length, double speedLimit);

    /*
     * This function updates the road; it calls the move function on all IVehicles currently on the road
     */
    void update();

    /*
     * enqueues a vehicle to the back of the queue
     */
    void enqueue(IVehicle* const vehicle);

    /*
     * dequeues the front vehicle
     */
    void dequeue();

    /*
     * This function checks if the road is empty
     * @return value bool
     */
    bool isEmpty();

    IVehicle* const getBackVehicle() const;

    Road* const getNextRoad() const;

    double getRoadLength() const;
    double getSpeedLimit() const;
    const std::string &getName() const;

private:
    double fRoadLength;
    double fSpeedLimit;
    std::string fName;

    Road* fNextRoad;
    std::deque<IVehicle*> fVehicles;

};


#endif //SIMULATION_ROAD_H
