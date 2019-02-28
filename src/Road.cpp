//============================================================================
// @name        : Road.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================
#include "Road.h"

Road *Road::getFNextRoad() const {
    return fNextRoad;
}

void Road::setFNextRoad(Road *fNextRoad) {
    Road::fNextRoad = fNextRoad;
}

const std::deque<IVehicle *> &Road::getFVehicles() const {
    return fVehicles;
}

void Road::setFVehicles(const std::deque<IVehicle *> &fVehicles) {
    Road::fVehicles = fVehicles;
}


void Road::update() {
    /*
     * This function updates the road; it calls the move function on all IVehicles currently on the road
     */
    for(int i=0; i < fVehicles.size();i++) {
        if (i != fVehicles.size() -1) fVehicles[i]->move(fVehicles[i+1]);
        else fVehicles[i]->move(fNextRoad->getFVehicles().back());
    }
}

bool Road::isEmpty() {
    /*
     * This function checks if the road is empty
     * @return value bool
     */
    return fVehicles.empty();
}



