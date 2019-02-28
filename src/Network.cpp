//============================================================================
// @name        : Network.cpp
// @author      : Mano Marichal
// @date        : 27.02.19
// @version     : 
// @copyright   : Project Software Engineering - BA1 Informatica - Mano Marichal - University of Antwerp
// @description : 
//============================================================================
#include "Network.h"
Network::Network() {
    fTimePassed=0;
}

const std::vector<Road *> &Network::getFRoads() const {
    return fRoads;
}

int Network::getFTimePassed() const {
    return fTimePassed;
}

void Network::setFTimePassed(int fTimePassed) {
    Network::fTimePassed = fTimePassed;
}

void Network::startSimulation() {
    /*
     * This function updates all roads and will exit when there are no IVehicles on the roads
     */
    while(true) {
        fTimePassed++;
        bool check = true;
        for (unsigned int i=0;i<fRoads.size();i++) {
            fRoads[i]->update();
            if (!fRoads[i]->isEmpty()) {
                check = false;
            }
        }
        if (check) {
            break;
        }
    }
}