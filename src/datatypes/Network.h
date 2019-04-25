//============================================================================
// @name        : Network.h
// @author      : Thomas Dooms
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
#include "../gui/gui.h"

class Network {

friend class NetworkExporter;

public:
    /**
     * ENSURE(this->properlyInitialized(), "Network constructor must end in properlyInitialized state");
     */
    Network(const std::vector<Road*>& roads);
    ~Network();

    bool properlyInitialized() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Network was not initialized when calling getTicksPassed");
     */
    int getTicksPassed() const;
    /**
     * REQUIRE(this->properlyInitialized(), "Network was not initialized when calling startSimulation");
     * REQUIRE(amountOfTicks >= 0, "Amount of ticks must be a positive integer");
     */
    void startSimulation(const Window* window, bool print = false, bool gui = false);

    /**
     * REQUIRE(this->properlyInitialized(), "Network was not initialized when calling startSimulation");
     */
    bool update();

    /**
     * REQUIRE(this->properlyInitialized(), "Network was not initialized when calling startSimulation");
     */
    bool checkWindow(Window::state state) const;

private:
    int fTicksPassed; // amount of ticks passed

    std::vector<Road*> fRoads;

    static const int fgkMaxTicks;

    Network* _initCheck;
};


#endif //SIMULATION_NETWORK_H
