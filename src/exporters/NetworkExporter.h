//============================================================================
// @name        : NetworkExporter.h
// @author      : Thomas Dooms, Ward Gauderis
// @date        : 3/26/19
// @version     : 2.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : Exports network to ofstreams
//============================================================================


#ifndef SIMULATION_NETWORKEXPORTER_H
#define SIMULATION_NETWORKEXPORTER_H

#include "../datatypes/Network.h"
#include <fstream>
#include <stdint.h>

class NetworkExporter {
public:

    /**
     *  REQUIRE(kNetwork, "Failed to export network: no network");
     *  ENSURE(res == 0 or res == 256, "Failed to create output directory");
     *  ENSURE(fgSimple.is_open(), "Failed to load file for simple output");
     *  ENSURE(fgImpression.is_open(), "Failed to load File for impression output");
     */
    static void init(const Network *network, const std::string &kSimplePath, const std::string &kImpressionPath);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling addSection");
     *  REQUIRE(kNetwork, "Failed to add section: no network");
     */
    static void addSection(const Network *kNetwork, uint32_t number);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling fin    ish");
     */
    static void finish();

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling tee");
     */
    template<class T>
    static void tee(const T &string, bool init);

    static std::string whitespace(int amount);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling printLane");
     */
    static void printLane(const std::vector<std::vector<char>> &lane, uint32_t max, uint32_t laneNum);


    static bool properlyInitialized();

private:
    static std::ofstream fgSimple;
    static std::ofstream fgImpression;

    static double fgScale;
    static uint32_t fgLongestName;

    static bool _initCheck;
};

#endif //SIMULATION_NETWORKEXPORTER_H
