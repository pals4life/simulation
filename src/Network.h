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

public:
    Network(std::vector<Road*> fRoads);

    int getTicksPassed() const;
    /*
     * This function updates all roads and will exit when there are no IVehicles on the roads
     */
    void startSimulation();

private:
    int fTicksPassed; // amount of ticks passed

    std::vector<Road*> fRoads;
};


#endif //SIMULATION_NETWORK_H
