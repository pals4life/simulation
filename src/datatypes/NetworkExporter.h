//============================================================================
// @name        : NetworkExporter.h
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef SIMULATION_NETWORKEXPORTER_H
#define SIMULATION_NETWORKEXPORTER_H

#include "Network.h"
#include <fstream>
#include <stdint.h>

class NetworkExporter
{
public:

    /**
     * ENSURE(this->properlyInitialized(), "constructor did not properlyInitialize");
     */
    NetworkExporter();

    /**
     * REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling printNetwork");
     * REQUIRE(network->properlyInitialized(), "Network was not initialized when calling printNetwork");
     *
     * ENSURE(fOutputFile.is_open(), "output file is not open");
     */
    void initialize(const Network* network, const std::string& name);

    /**
     * REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling finish");
     * REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling finish");
     */
    void addSection(uint32_t number);

    /**
     * REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling addSection");
     * REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling addSection");
     */
    void finish();

    bool properlyInitialized();

private:
    std::ofstream fOutputFile;
    const Network* kfNetwork;

    const NetworkExporter* _initCheck;
    bool fIsInitialized;
};


#endif //SIMULATION_NETWORKEXPORTER_H
