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
#include <fstream>
#include "Road.h"

class Network {

public:
    Network(const std::vector<Road*>& roads);
    ~Network();

    int getTicksPassed() const;
    /*
     * This function updates all roads and will exit when there are no IVehicles on the roads or the amout of ticks has run out
     * @ param ticks: the amount of ticks the simulation has to carry out
     */
    void startSimulation(int amountOfTicks = fgkMaxTicks);



private:
    int fTicksPassed; // amount of ticks passed

    std::vector<Road*> fRoads;

    /*
     * This function prints the current state of the network
     */
    void printNetwork();

    static const int fgkMaxTicks;

};


#endif //SIMULATION_NETWORK_H
