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
    static std::string addSection(const Network *kNetwork, uint32_t number);

    static void cgExport(const Network *kNetwork, uint32_t number);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling finish");
     */
    static void finish();

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling tee");
     */
    template<typename T>
    static void tee(const T &string, bool init) {
        REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling tee");
        fgImpression << string;
        std::cout << string;
        fgBuf << string;
        if (init) fgSimple << string;
    }

    static std::string whitespace(int amount);

    /**
     *  REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling printLane");
     */
    static void printLane(const std::vector<std::vector<char>> &lane, uint32_t max, uint32_t laneNum);


    static bool properlyInitialized();

private:
    static std::ofstream fgSimple;
    static std::ofstream fgImpression;

    static std::stringstream fgBuf;

    static double fgScale;
    static uint32_t fgLongestName;

    static bool _initCheck;

    static void general(std::ofstream& ini);
};

#endif //SIMULATION_NETWORKEXPORTER_H
