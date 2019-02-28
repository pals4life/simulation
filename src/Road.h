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
private:
    Road* fNextRoad;

    std::deque<IVehicle*> fVehicles;

public:
    Road(Road* next) {fNextRoad = next;};

    Road *getFNextRoad() const;

    void setFNextRoad(Road *fNextRoad);

    const std::deque<IVehicle *> &getFVehicles() const;

    void setFVehicles(const std::deque<IVehicle *> &fVehicles);

    void update();

    bool isEmpty();


};


#endif //SIMULATION_ROAD_H
