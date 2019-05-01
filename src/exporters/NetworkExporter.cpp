//============================================================================
// @name        : NetworkExporter.cpp
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "NetworkExporter.h"
#include "../DesignByContract.h"
#include <stdlib.h>
#include <math.h>

std::ofstream NetworkExporter::fgSimple;
std::ofstream NetworkExporter::fgImpression;

double NetworkExporter::fgScale = 0;
uint32_t NetworkExporter::fgLongestName = 0;

bool NetworkExporter::_initCheck = false;

void
NetworkExporter::init(const Network *kNetwork, const std::string &kSimplePath, const std::string &kImpressionPath) {
    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "Could not make directory");
    fgSimple.open(("outputfiles/" + kSimplePath + ".txt").c_str());
    ENSURE(fgSimple.is_open(), "Output file is not open");

    fgImpression.open(("outputfiles/" + kImpressionPath + ".txt").c_str());
    ENSURE(fgImpression.is_open(), "Output file is not open");

    double maxLength = 0;
    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        Road *road = kNetwork->fRoads[i];

        tee("Baan : " + road->getName() + '\n', true);
        tee("  -> Snelheidslimiet: " + std::to_string(int(std::round(road->getSpeedLimit() * 3.6))) + "km/u\n", true);
        tee("  -> Lengte         : " + std::to_string(int(std::round(road->getRoadLength()))) + "m\n", true);
        for (const auto &zone: road->getZones()) {
            tee("  -> zone           : Begin: " + std::to_string(int(std::round(zone->getPosition()))) +
                "m Snelheidslimiet: " + std::to_string(int(std::round(zone->getSpeedlimit() * 3.6))) + "km/u\n", true);
        }
        for (const auto &busStop: road->getBusStops()) {
            tee("  -> Bushalte       : Positie: " + std::to_string(int(std::round(busStop->getPosition()))) + "m\n",
                true);
        }
        for (const auto &trafficLight: road->getTrafficLights()) {
            tee("  -> Verkeerslicht  : Positie: " + std::to_string(int(std::round(trafficLight->getPosition()))) +
                "m\n", true);
        }

        if (road->getRoadLength() > maxLength) maxLength = road->getRoadLength();
        if (road->getName().size() > fgLongestName) fgLongestName = road->getName().size();
    }
    fgScale = maxLength / 120;
    tee("\n-------------------------------------------------\nOne character is " + std::to_string(fgScale) + " meters",
        false);
    _initCheck = true;
}

void NetworkExporter::finish() {
    fgImpression << std::flush;
    fgSimple << std::flush;
    fgImpression.close();
    fgSimple.close();
}

void NetworkExporter::addSection(const Network *kNetwork, uint32_t number) {
    fgSimple << "\n-------------------------------------------------\n";
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

    tee("\n-------------------------------------------------\n", false);
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
    for (uint32_t l = 0; l < max; ++l) {
        if (laneNum != 0) tee(whitespace(fgLongestName + 3), false);
        if (l == 0) tee(std::to_string(laneNum + 1) + " ", false);
        else tee(whitespace(fgLongestName + 4 + std::to_string(laneNum + 1).size()), false);
        for (uint32_t k = 0; k < lane.size(); ++k) {
            tee((l < lane[k].size() ? lane[k][l] : (l == 0 ? '=' : ' ')), false);
        }
        tee('\n', false);
    }
}

template<class T>
void NetworkExporter::tee(const T &string, bool init) {
    fgImpression << string;
    std::cout << string;
    if (init) fgSimple << string;
}

bool NetworkExporter::properlyInitialized() const {
    return _initCheck;
}
