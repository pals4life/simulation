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

#include "../datatypes/Network.h"
#include <fstream>
#include <stdint.h>

class NetworkExporter {
public:

    /**
     * REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling printNetwork");
     * REQUIRE(network->properlyInitialized(), "Network was not initialized when calling printNetwork");
     *
     * ENSURE(fOutputFile.is_open(), "output file is not open");
     */
    static void init(const Network *network, const std::string &kSimplePath, const std::string &kImpressionPath);

    /**
     * REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling finish");
     * REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling finish");
     */
    static void addSection(const Network* kNetwork, uint32_t number);

    /**
     * REQUIRE(this->properlyInitialized(), "NetworkExporter was not constructed when calling addSection");
     * REQUIRE(this->fIsInitialized, "NetworkExporter was not initialized when calling addSection");
     */
    static void finish();

private:
    static std::string whitespace(int amount);

    static void printLane(const std::vector<std::vector<char>> &lane, uint32_t max, uint32_t laneNum);

    static std::ofstream fSimple;
    static std::ofstream fImpression;

    static double scale;
    static uint32_t longestName;
};


#endif //SIMULATION_NETWORKEXPORTER_H
