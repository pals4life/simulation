//============================================================================
// @name        : NetworkExporter.cpp
// @author      : Thomas Dooms, Ward Gauderis
// @date        : 3/26/19
// @version     : 2.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : Exports network to ofstreams
//============================================================================

#include "NetworkExporter.h"
#include "../DesignByContract.h"
#include <stdlib.h>
#include <math.h>
#include <sstream>

std::ofstream NetworkExporter::fgSimple;
std::ofstream NetworkExporter::fgImpression;

std::stringstream NetworkExporter::fgBuf;

double NetworkExporter::fgScale = 0;
uint32_t NetworkExporter::fgLongestName = 0;

bool NetworkExporter::_initCheck = false;

void
NetworkExporter::init(const Network *kNetwork, const std::string &kSimplePath, const std::string &kImpressionPath) {
    REQUIRE(kNetwork, "Failed to export network: no network");
    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "Failed to create output directory");

    fgSimple.open(("outputfiles/" + kSimplePath + ".txt").c_str());
    ENSURE(fgSimple.is_open(), "Failed to load file for simple output");

    fgImpression.open(("outputfiles/" + kImpressionPath + ".txt").c_str());
    ENSURE(fgImpression.is_open(), "Failed to load File for impression output");

    _initCheck = true;

    double maxLength = 0;
    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        Road *road = kNetwork->fRoads[i];

        tee("Baan : " + road->getName() + '\n', true);
        tee("  -> Snelheidslimiet: " + std::to_string(int(std::round(road->getSpeedLimit() * 3.6))) + "km/u\n", true);
        tee("  -> Lengte         : " + std::to_string(int(std::round(road->getRoadLength()))) + "m\n", true);
        for (uint32_t j = 0; j < road->getZones().size(); j++) {
            const Zone *zone = road->getZones()[j];
            tee("  -> zone           : Begin: " + std::to_string(int(std::round(zone->getPosition()))) +
                "m Snelheidslimiet: " + std::to_string(int(std::round(zone->getSpeedlimit() * 3.6))) + "km/u\n", true);
        }
        for (uint32_t j = 0; j < road->getBusStops().size(); j++) {
            const BusStop *busStop = road->getBusStops()[j];
            tee("  -> Bushalte       : Positie: " + std::to_string(int(std::round(busStop->getPosition()))) + "m\n",
                true);
        }
        for (uint32_t j = 0; j < road->getTrafficLights().size(); j++) {
            const TrafficLight *trafficLight = road->getTrafficLights()[j];
            tee("  -> Verkeerslicht  : Positie: " + std::to_string(int(std::round(trafficLight->getPosition()))) +
                "m\n", true);
        }

        if (road->getRoadLength() > maxLength) maxLength = road->getRoadLength();
        if (road->getName().size() > fgLongestName) fgLongestName = road->getName().size();
    }
    fgScale = maxLength / 120;
    tee("\n", true);
    tee("-------------------------------------------------\nOne character is " + std::to_string(fgScale) + " meters\n",
        false);
}

void NetworkExporter::finish() {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling finish");
    fgImpression << std::flush;
    fgSimple << std::flush;
    fgImpression.close();
    fgSimple.close();
    fgScale = 0;
    fgLongestName = 0;
    _initCheck = false;
}

std::string NetworkExporter::addSection(const Network *kNetwork, uint32_t number) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling addSection");
    REQUIRE(kNetwork, "Failed to add section: no network");
    fgSimple << "-------------------------------------------------\n";
    if (number != 1) fgSimple << "State of the network after " << number << " ticks have passed:\n\n";
    else fgSimple << "State of the network after " << number << " tick has passed:\n\n";
    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        const Road *road = kNetwork->fRoads[i];
        for (uint32_t j = 0; j < road->getNumLanes(); j++) {
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                fgSimple << "Voertuig: " + vehicle->getType() + '(' + vehicle->getLicensePlate() + ")\n";
                fgSimple << "  -> Baan    : " << road->getName() << '\n';
                fgSimple << "  -> Positie : " << vehicle->getPosition() << '\n';
                fgSimple << "  -> Snelheid: " << vehicle->getVelocity() * 3.6 << '\n';
            }
        }
    }
    fgSimple << '\n';

    fgBuf.str("");

    tee("-------------------------------------------------\n", false);
    if (number != 1) tee("State of the network after " + std::to_string(number) + " ticks have passed:\n\n", false);
    else tee("State of the network after " + std::to_string(number) + " tick has passed:\n\n", false);
    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        const Road *road = kNetwork->fRoads[i];
        tee(road->getName() + whitespace(fgLongestName - road->getName().size()) + " | ", false);
        for (uint32_t j = 0; j < road->getNumLanes(); j++) {
            std::vector<std::vector<char> > lane;
            uint32_t max = 0;
            lane.resize(static_cast<uint32_t >(floor(road->getRoadLength() / fgScale) + 1));
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                uint32_t pos = static_cast<uint32_t >(floor(vehicle->getPosition() / fgScale));
                lane[pos].push_back(toupper(vehicle->getType()[0]));
                if (lane[pos].size() > max) max = lane[pos].size();
            }
            if (max == 0) {
                printLane(lane, 1, j);
            } else {
                printLane(lane, max, j);
            }
        }
    }
    tee("\n", false);

    return fgBuf.str();
}

std::string NetworkExporter::whitespace(const int amount) {
    std::string white;
    for (int i = 0; i < amount; ++i) {
        white += " ";
    }
    return white;
}

void
NetworkExporter::printLane(const std::vector<std::vector<char>> &lane, const uint32_t max, const uint32_t laneNum) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling printLane");
    for (uint32_t l = 0; l < max; ++l) {
        if (!(laneNum == 0 && l == 0)) tee(whitespace(fgLongestName + 3), false);
        if (l == 0) tee(std::to_string(laneNum + 1) + " ", false);
        else tee(whitespace(1 + std::to_string(laneNum + 1).size()), false);
        for (uint32_t k = 0; k < lane.size(); ++k) {
            tee((l < lane[k].size() ? lane[k][l] : (l == 0 ? '=' : ' ')), false);
        }
        tee('\n', false);
    }
}

bool NetworkExporter::properlyInitialized() {
    return _initCheck;
}
