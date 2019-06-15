#include <utility>

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
#include <iomanip>

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

void NetworkExporter::cgExport(const Network *kNetwork, uint32_t number) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling cgExport");
    REQUIRE(kNetwork, "Failed to export to cg: no network");

    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "Failed to create output directory");

    const std::string kFilename = "outputfiles/" + std::to_string(number) + ".ini";
    std::ofstream ini(kFilename);
    ENSURE(ini.is_open(), "Failed to open file for cg export");

    ini << std::fixed;
    ini << std::setprecision(2);

    double y = 0;

    int nr = 0;
    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        const Road *road = kNetwork->fRoads[i];
        for (uint32_t j = 0; j < road->getNumLanes(); j++) {
            std::vector<std::vector<char> > scaleLane;
            uint32_t max = 0;
            scaleLane.resize(static_cast<uint32_t >(floor(road->getRoadLength() / fgScale) + 1));
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                uint32_t pos = static_cast<uint32_t >(floor(vehicle->getPosition() / fgScale));
                scaleLane[pos].push_back(toupper(vehicle->getType()[0]));
                if (scaleLane[pos].size() > max) max = scaleLane[pos].size();
            }
            if (max == 0) max = 1;
            lane(ini, nr, max, y, scaleLane);
            y += max * 2 + 1;
        }
        y += 1;
    }

    general(ini, nr);

    ini.close();
    const std::string kCommand = "(./engine/engine " + kFilename + ")"; //  " && rm " + kFilename +
    system(kCommand.c_str());
    exit(0);
}

void NetworkExporter::general(std::ofstream &ini, const int &nr) {
    ini << "[General]\n"
           "size = 4096\n"
           "backgroundcolor = (0.05,0.06,0.05)\n"
           "type = \"LightedZBuffering\"\n"
           "nrLights = 1\n"
           "shadowEnabled = FALSE\n"
           "eye = (0,0,120)\n"
           "nrFigures = " << nr << "\n"
                                   "\n"
                                   "[Light0]\n"
                                   "infinity = TRUE\n"
                                   "direction = (0,0,-100)\n"
                                   "ambientLight = (0.4,0.4,0.4)\n"
                                   "diffuseLight = (0.8,0.8,0.8)\n"
                                   "specularLight = (0.8,0.8,0.8)\n"
                                   "\n";
}

void NetworkExporter::car(std::ofstream &ini, int &nr, const Pos &pos) {
    Object bottom = Object::rectangle(pos, {pos.fX - 3, pos.fY - 1.5, pos.fZ + 0.5});
    bottom.fAmbient = {0.5, 0.1, 0.1};
    bottom.fDiffuse = {0.5, 0.1, 0.1};
    bottom.fSpecular = {0.8, 0.1, 0.1};
    bottom.fReflectionCoefficient = 20;
    Object top = Object::rectangle({pos.fX - 1, pos.fY, pos.fZ + 0.5}, {pos.fX - 2, pos.fY - 1.5, pos.fZ + 1});
    top.fAmbient = {0.1, 0.1, 0.1};
    top.fDiffuse = {0.3, 0.1, 0.3};
    top.fSpecular = {0.3, 0.3, 1};
    top.fReflectionCoefficient = 20;
    bottom.print(ini, nr++);
    top.print(ini, nr++);
    wheel(ini, nr, {pos.fX - 0.5, pos.fY + 0.2, pos.fZ});
    wheel(ini, nr, {pos.fX - 2.5, pos.fY + 0.2, pos.fZ});
}

void NetworkExporter::wheel(std::ofstream &ini, int &nr, const Pos &kPos) {
    ini << "[Figure" << nr++ << "]\n";
    ini << "type = \"Cylinder\"\n";
    ini << "scale = 0.25\n";
    ini << "rotateX = 90\n";
    ini << "center = " << kPos << "\n";
    ini << "ambientReflection = (0.1, 0.1, 0.1)\n";
    ini << "diffuseReflection = (0.1, 0.1, 0.1)\n";
    ini << "n = 10\n";
    ini << "height = 8\n";
    ini << "\n";
}

void NetworkExporter::lane(std::ofstream &ini, int &nr, double max, double y,
                           const std::vector<std::vector<char>> &scaleLane) {
    ini << "[Figure" << nr++ << "]\n";
    ini << "type = \"LineDrawing\"\n";
    ini << "ambientReflection = (0.2, 0.2, 0.2)\n";
    ini << "diffuseReflection = (0.2, 0.2, 0.2)\n";
    ini << "nrPoints = 4\n";
    ini << "nrLines = 1\n";
    ini << "point0 = " << Pos{y, 0, 0} << "\n";
    ini << "point1 = " << Pos{y + (max * 2), 0, 0} << "\n";
    ini << "point2 = " << Pos{y + (max * 2), scaleLane.size() * 10.0, 0} << "\n";
    ini << "point3 = " << Pos{y, scaleLane.size() * 10.0, 0} << "\n";
    ini << "line0 = (0,1,2,3)\n";
    ini << "\n";
    for (unsigned int i = 0; i < max; ++i) {
        for (unsigned int j = 0; j < scaleLane.size(); ++j) {
            if (i >= scaleLane[j].size()) continue;
            if (scaleLane[j][i] == 'A') {
                car(ini, nr, {y, j * 10.0, 0});
            }
        }
        y += 2;
    }
}

Object Object::rectangle(const Pos &begin, const Pos &end) {
    Object object;
    object.fPoints.reserve(8);
    object.fPoints.push_back({end.fX, begin.fY, begin.fZ});
    object.fPoints.push_back({begin.fX, end.fY, begin.fZ});
    object.fPoints.push_back({end.fX, end.fY, end.fZ});
    object.fPoints.push_back({begin.fX, begin.fY, end.fZ});
    object.fPoints.push_back({end.fX, end.fY, begin.fZ});
    object.fPoints.push_back({begin.fX, begin.fY, begin.fZ});
    object.fPoints.push_back({end.fX, begin.fY, end.fZ});
    object.fPoints.push_back({begin.fX, end.fY, end.fZ});
    object.fFaces.reserve(6);
    object.fFaces.emplace_back(Face({0, 4, 2, 6}));
    object.fFaces.emplace_back(Face({4, 1, 7, 2}));
    object.fFaces.emplace_back(Face({1, 5, 3, 7}));
    object.fFaces.emplace_back(Face({5, 0, 6, 3}));
    object.fFaces.emplace_back(Face({6, 2, 7, 3}));
    object.fFaces.emplace_back(Face({0, 5, 1, 4}));
    return object;
}

void Object::print(std::ofstream &ini, const int nr) {
    ini << "[Figure" << nr << "]\n";
    ini << "type = \"LineDrawing\"\n";
    ini << "ambientReflection = " << fAmbient << "\n";
    ini << "diffuseReflection = " << fDiffuse << "\n";
    ini << "specularReflection = " << fSpecular << "\n";
    ini << "reflectionCoefficient = " << fReflectionCoefficient << "\n";
    ini << "nrPoints = " << fPoints.size() << "\n";
    ini << "nrLines = " << fFaces.size() << "\n";
    for (unsigned int i = 0; i < fPoints.size(); i++) {
        ini << "point" << i << " = " << fPoints[i] << "\n";
    }
    for (unsigned int i = 0; i < fFaces.size(); i++) {
        ini << "line" << i << " = " << fFaces[i] << "\n";
    }
    ini << "\n";
}

std::ostream &operator<<(std::ostream &os, const Color &color) {
    os << "(" << color.fR << "," << color.fG << "," << color.fB << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Pos &pos) {
    os << "(" << pos.fX << "," << pos.fY << "," << pos.fZ << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Face &face) {
    os << "(";
    for (unsigned int i = 0; i < face.fIndexes.size() - 1; ++i) {
        os << face.fIndexes[i] << ",";
    }
    os << face.fIndexes.back() << ")";
    return os;
}

Face::Face(std::vector<int> fIndexes) : fIndexes(std::move(fIndexes)) {}
