//============================================================================
// @name        : Network.h
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================
#ifndef SIMULATION_NETWORK_H
#define SIMULATION_NETWORK_H

#include <vector>
#include "Road.h"

class Network {
private:
    int fTimePassed;                              // time passed in seconds

    std::vector<Road*> fRoads;

public:
    int getFTimePassed() const;

    void setFTimePassed(int fTimePassed);

    const std::vector<Road *> &getFRoads() const;

    void startSimulation();

};


#endif //SIMULATION_NETWORK_H
