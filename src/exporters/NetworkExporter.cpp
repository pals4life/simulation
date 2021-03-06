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
#include <cfloat>
#include <sys/stat.h>

typedef Object object;
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
            uint32_t max = 1;
            lane.resize(static_cast<uint32_t >(ceil(road->getRoadLength() / fgScale)));
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                uint32_t pos = static_cast<uint32_t >(floor(vehicle->getPosition() / fgScale));
                lane[pos].push_back(toupper(vehicle->getType()[0]));
                if (lane[pos].size() > max) max = lane[pos].size();
            }
            printLane(lane, max, j);
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

void NetworkExporter::cgExport(const Network *kNetwork, const unsigned int kTick) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling cgExport");
    REQUIRE(kNetwork, "Failed to export to cg: no network");
    REQUIRE(FileExists("engine/engine"), "Failed to export to cg engine: engine not found");

    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "Failed to create output directory");

    std::string filename = "outputfiles/cg.ini";
    if (kTick > 0) filename = "outputfiles/tick" + std::to_string(kTick) + ".ini";
    std::ofstream ini(filename);
    ENSURE(ini.is_open(), "Failed to open file for cg export");

    ini << std::fixed;
    ini << std::setprecision(2);

    double y = 0;
    int nr = 0;

    for (uint32_t i = 0; i < kNetwork->fRoads.size(); i++) {
        const Road *road = kNetwork->fRoads[i];
        for (uint32_t j = 0; j < road->getNumLanes(); j++) {
            uint32_t max = 1;
            double roadLength = road->getRoadLength() / fgScale;
            double prevPosition = DBL_MAX;
            for (uint32_t k = 0; k < (*road)[j].size(); k++) {
                const IVehicle *vehicle = (*road)[j][k];
                double position = vehicle->getPosition() / fgScale;
                char type = vehicle->getType()[0];
                double length = vehicle->getVehicleLength();
                if (position >= prevPosition) {
                    max++;
                }
                switch (type) {
                    case 'a':
                        car(ini, nr, {-position, y + (max - 1) * 2, 0.25}, true);
                        break;
                    case 'b':
                        bus(ini, nr, {-position, y + (max - 1) * 2, 0.25});
                        break;
                    case 'v':
                        truck(ini, nr, {-position, y + (max - 1) * 2, 0.25});
                        break;
                    case 'm':
                        motorcycle(ini, nr, {-position, (y + (max - 1) * 2) + 0.5, 0.25});
                        break;
                    default:
                        std::cerr << "Vehicle type can not be represented with the CG engine\n";
                }
                prevPosition = position - length;
            }
            lane(ini, nr, max, y, roadLength);
            if (j != road->getNumLanes() - 1) line(ini, nr, y + 2.0 * max, roadLength);
            y += max * 2.0 + 0.5;
        }
        for (unsigned int b = 0; b < road->getBusStops().size(); b++) {
            double pos = road->getBusStops()[b]->getPosition() / fgScale;
            sign(ini, nr, pos, y, 'y');
        }
        for (unsigned int b = 0; b < road->getTrafficLights().size(); b++) {
            double pos = road->getTrafficLights()[b]->getPosition() / fgScale;
            TrafficLight::EColor color = road->getTrafficLights()[b]->getColor();
            char c;
            switch (color) {
                case TrafficLight::EColor::kGreen:
                    c = 'g';
                    break;
                case TrafficLight::EColor::kOrange:
                    c = 'o';
                    break;
                case TrafficLight::EColor::kRed:
                    c = 'r';
                    break;
            }
            sign(ini, nr, pos, y, c);
        }
        for (unsigned int b = 0; b < road->getZones().size(); b++) {
            double pos = road->getZones()[b]->getPosition() / fgScale;
            sign(ini, nr, pos, y, 'w');
        }
        y += 1;
    }

    general(ini, nr);

    ini.close();
    std::string command = "./engine/engine " + filename + "";
    if (kTick > 0) command += " &";
    res = system(command.c_str());
    ENSURE(res == 0 or res == 256, "Failed to run engine on the generated ini file");
    ENSURE(!ini.is_open(), "Failed to close ofstream to ini");
}

void NetworkExporter::general(std::ofstream &ini, const int &kNr) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling general");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(kNr >= 0, "kNr must be greater than 0");
    ini << "[General]\n"
           "size = 2024\n"
           "backgroundcolor = (0.05,0.06,0.05)\n"
           "type = \"LightedZBuffering\"\n"
           "nrLights = 1\n"
           "shadowEnabled = FALSE\n"
           "eye = (0,50,100)\n"
           "nrFigures = " << kNr << "\n"
                                    "\n"
                                    "[Light0]\n"
                                    "infinity = TRUE\n"
                                    "direction = (-3,-1,-10)\n"
                                    "ambientLight = (0.4,0.4,0.4)\n"
                                    "diffuseLight = (1,1,1)\n"
                                    "specularLight = (1,1,1)\n"
                                    "\n";
}

void NetworkExporter::car(std::ofstream &ini, int &nr, const Pos &pos, bool real) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling car");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    Object bottom = Object::rectangle(pos, {pos.fX + 3, pos.fY + 1.5, pos.fZ + 0.5});
    Object top = Object::rectangle({pos.fX + 1, pos.fY, pos.fZ + 0.5}, {pos.fX + 2, pos.fY + 1.5, pos.fZ + 1});
    if (real) {
        bottom.fAmbient.set(0.5, 0.1, 0.1);
        bottom.fDiffuse.set(1, 0.1, 0.1);
        bottom.fSpecular.set(1, 0.1, 0.1);
        bottom.fReflectionCoefficient = 20;
        top.fAmbient.set(0.1, 0.1, 0.1);
        top.fDiffuse.set(0.8, 0.1, 0.8);
        top.fSpecular.set(0.3, 0.3, 1);
        top.fReflectionCoefficient = 20;
    } else {
        bottom.fAmbient.set(0.5, 0.1, 0.5);
        bottom.fDiffuse.set(1, 0.1, 1);
        bottom.fSpecular.set(0.8, 0.1, 0.8);
        bottom.fReflectionCoefficient = 20;
        top.fAmbient.set(0.5, 0.0, 0.5);
        top.fDiffuse.set(0.4, 0.0, 0.3);
        top.fSpecular.set(0.1, 0.0, 0.2);
        top.fReflectionCoefficient = 20;

    }
    bottom.print(ini, nr++);
    top.print(ini, nr++);
    wheel(ini, nr, {pos.fX + 0.5, pos.fY - 0.25, pos.fZ});
    wheel(ini, nr, {pos.fX + 2.5, pos.fY - 0.25, pos.fZ});
}

void NetworkExporter::wheel(std::ofstream &ini, int &nr, const Pos &kPos) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling wheel");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    ini << "[Figure" << nr++ << "]\n";
    ini << "type = \"Cylinder\"\n";
    ini << "scale = 0.25\n";
    ini << "rotateX = -90\n";
    ini << "center = " << kPos << "\n";
    ini << "ambientReflection = (0.1, 0.1, 0.1)\n";
    ini << "diffuseReflection = (0.1, 0.1, 0.1)\n";
    ini << "n = 10\n";
    ini << "height = 8\n";
    ini << "\n";
}

void NetworkExporter::lane(std::ofstream &ini, int &nr, double max, double y, double roadlength) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling lane");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    ini << "[Figure" << nr++ << "]\n";
    ini << "type = \"LineDrawing\"\n";
    ini << "ambientReflection = (0.2, 0.2, 0.2)\n";
    ini << "diffuseReflection = (0.3, 0.3, 0.3)\n";
    ini << "specularReflection = (0.2, 0.2, 0.2)\n";
    ini << "reflectoinCoefficient = 10\n";
    ini << "nrPoints = 4\n";
    ini << "nrLines = 1\n";
    ini << "point0 = " << Pos{0, y, 0} << "\n";
    ini << "point1 = " << Pos{0, y + (max * 2), 0} << "\n";
    ini << "point2 = " << Pos{-roadlength, y + (max * 2), 0} << "\n";
    ini << "point3 = " << Pos{-roadlength, y, 0} << "\n";
    ini << "line0 = (0,1,2,3)\n";
    ini << "\n";
}

void NetworkExporter::bus(std::ofstream &ini, int &nr, const Pos &pos) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling bus");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    Object bottom = Object::rectangle(pos, {pos.fX + 10, pos.fY + 1.5, pos.fZ + 3});
    Object window = Object::rectangle({pos.fX + 0.5, pos.fY - 0.02, pos.fZ + 0.5},
                                      {pos.fX + 1.25, pos.fY + 1.52, pos.fZ + 2.5});
    window.fDiffuse.set(1, 0.6, 0.2);
    bottom.fAmbient.set(0.5, 0.3, 0.1);
    bottom.fDiffuse.set(0.8, 0.8, 0.1);
    bottom.fSpecular.set(0.5, 0.5, 0.1);
    bottom.fReflectionCoefficient = 20;
    window.print(ini, nr++);
    bottom.print(ini, nr++);
    wheel(ini, nr, {pos.fX + 5, pos.fY - 0.25, pos.fZ});
    wheel(ini, nr, {pos.fX + 2, pos.fY - 0.25, pos.fZ});
    wheel(ini, nr, {pos.fX + 8, pos.fY - 0.25, pos.fZ});
}

void NetworkExporter::truck(std::ofstream &ini, int &nr, const Pos &pos) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling truck");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    Object cabin = Object::rectangle({pos.fX, pos.fY, pos.fZ + 1}, {pos.fX + 1, pos.fY + 1.5, pos.fZ + 3});
    cabin.fAmbient.set(0.5, 0.1, 0.1);
    cabin.fDiffuse.set(0.8, 0.1, 0.1);
    cabin.fSpecular.set(0.8, 0.1, 0.);
    cabin.fReflectionCoefficient = 20;
    Object bottom = Object::rectangle(pos, {pos.fX + 15, pos.fY + 1.5, pos.fZ + 1});
    bottom.fAmbient.set(0.1, 0.1, 0.1);
    bottom.fDiffuse.set(0.3, 0.3, 0.3);
    bottom.fSpecular.set(0.1, 0.1, 0.1);
    bottom.fReflectionCoefficient = 20;
    Object container = Object::rectangle({pos.fX + 2, pos.fY, pos.fZ + 1}, {pos.fX + 8, pos.fY + 1.5, pos.fZ + 3});
    container.fAmbient.set(0.1, 0.3, 0.5);
    container.fDiffuse.set(0.1, 0.8, 0.8);
    container.fSpecular.set(0.1, 0.5, 0.5);
    container.fReflectionCoefficient = 20;
    car(ini, nr, {pos.fX + 10, pos.fY, pos.fZ + 1.25}, false);
    bottom.print(ini, nr++);
    container.print(ini, nr++);
    cabin.print(ini, nr++);
    wheel(ini, nr, {pos.fX + 1.5, pos.fY - 0.25, pos.fZ});
    wheel(ini, nr, {pos.fX + 5, pos.fY - 0.25, pos.fZ});
    wheel(ini, nr, {pos.fX + 10, pos.fY - 0.25, pos.fZ});
    wheel(ini, nr, {pos.fX + 15 - 1.5, pos.fY - 0.25, pos.fZ});
}

void NetworkExporter::motorcycle(std::ofstream &ini, int &nr, const Pos &pos) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling motorcycle");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    Object bottom = Object::rectangle(pos, {pos.fX + 1, pos.fY + 0.5, pos.fZ + 0.5});
    Object seat = Object::rectangle({pos.fX + 0.5, pos.fY + 0.05, pos.fZ + 0.5},
                                    {pos.fX + 0.9, pos.fY + 0.45, pos.fZ + 0.75});
    Object steer = Object::rectangle({pos.fX, pos.fY - 0.1, pos.fZ + 0.5}, {pos.fX + 0.1, pos.fY + 0.6, pos.fZ + 0.75});
    bottom.fAmbient.set(0.5, 0.5, 0.5);
    bottom.fDiffuse.set(0.1, 1, 0.1);
    bottom.fSpecular.set(0.1, 0.7, 0.1);
    bottom.fReflectionCoefficient = 20;
    seat.fAmbient.set(0.1, 0.1, 0.1);
    seat.fDiffuse.set(0.1, 0.1, 0.1);
    steer.fAmbient.set(0.1, 0.1, 0.1);
    steer.fDiffuse.set(0.1, 0.1, 0.1);
    steer.print(ini, nr++);
    bottom.print(ini, nr++);
    seat.print(ini, nr++);
}

void NetworkExporter::line(std::ofstream &ini, int &nr, const double &y, const double &x) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling line");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    double now = 0;
    bool white = true;
    while (-now < x) {
        ini << "[Figure" << nr++ << "]\n";
        ini << "type = \"LineDrawing\"\n";
        if (white) {
            ini << "ambientReflection = (0.9,0.9,0.9)\n";
            ini << "diffuseReflection = (0.9,0.9,0.9)\n";
            ini << "specularReflection = (0.9,0,9.9)\n";
            ini << "reflectoinCoefficient = 10\n";
        } else {
            ini << "ambientReflection = (0.2, 0.2, 0.2)\n";
            ini << "diffuseReflection = (0.3, 0.3, 0.3)\n";
            ini << "specularReflection = (0.2, 0.2, 0.2)\n";
            ini << "reflectoinCoefficient = 10\n";
        }
        white = !white;
        ini << "nrPoints = 4\n";
        ini << "nrLines = 1\n";
        ini << "point0 = " << Pos{now, y, 0} << "\n";
        ini << "point1 = " << Pos{now, y + 0.5, 0} << "\n";
        if (now + x <= 2) {
            now = -x;
        } else {
            now -= 2;
        }
        ini << "point2 = " << Pos{now, y + 0.5, 0} << "\n";
        ini << "point3 = " << Pos{now, y, 0} << "\n";
        ini << "line0 = (0,1,2,3)\n";
        ini << "\n";
    }
}

void NetworkExporter::sign(std::ofstream &ini, int &nr, const double &x, const double &y, char c) {
    REQUIRE(properlyInitialized(), "NetworkExporter was not initialized when calling sign");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
    Object pole = Object::rectangle({-x, y, 0}, {-x - 0.1, y + 0.1, 2});
    pole.fDiffuse.set(0.3, 0.3, 0.3);
    pole.fSpecular.set(0.3, 0.3, 0.3);
    pole.print(ini, nr++);
    ini << "[Figure" << nr++ << "]\n";
    ini << "type = \"Sphere\"\n";
    switch (c) {
        case 'r':
            ini << "ambientReflection = (1,0,0)\n";
            ini << "diffuseReflection = (1,0,0)\n";
            break;
        case 'o':
            ini << "ambientReflection = (1, 0.5, 0)\n";
            ini << "diffuseReflection = (1, 0.5, 0)\n";
            break;
        case 'g':
            ini << "ambientReflection = (0,1,0)\n";
            ini << "diffuseReflection = (0,1,0)\n";
            break;
        case 'y':
            ini << "ambientReflection = (1,1,0)\n";
            ini << "diffuseReflection = (1,1,0)\n";
            break;
        case 'w':
            ini << "ambientReflection = (1,1,1)\n";
            ini << "diffuseReflection = (1,1,1)\n";
            break;
        default:
            std::cerr << "Sign of type " << c << " could not be displayed\n";
    }
    ini << "scale = 0.4\n";
    ini << "center = " << Pos{-x - 0.05, y, 2} << "\n";
    ini << "n = 1\n";
    ini << "\n";
}

Object Object::rectangle(const Pos &begin, const Pos &end) {
    REQUIRE(!(begin.fX == end.fX && begin.fY == end.fY && begin.fZ == end.fZ),
            "NetworkExporter was not initialized when calling sign");
    Object object;
    object.fPoints.reserve(8);
    object.fPoints.emplace_back(end.fX, begin.fY, begin.fZ);
    object.fPoints.emplace_back(begin.fX, end.fY, begin.fZ);
    object.fPoints.emplace_back(end.fX, end.fY, end.fZ);
    object.fPoints.emplace_back(begin.fX, begin.fY, end.fZ);
    object.fPoints.emplace_back(end.fX, end.fY, begin.fZ);
    object.fPoints.emplace_back(begin.fX, begin.fY, begin.fZ);
    object.fPoints.emplace_back(end.fX, begin.fY, end.fZ);
    object.fPoints.emplace_back(begin.fX, end.fY, end.fZ);
    object.fFaces.reserve(6);
    object.fFaces.emplace_back(std::vector<int>{0, 4, 2, 6});
    object.fFaces.emplace_back(std::vector<int>{4, 1, 7, 2});
    object.fFaces.emplace_back(std::vector<int>{1, 5, 3, 7});
    object.fFaces.emplace_back(std::vector<int>{5, 0, 6, 3});
    object.fFaces.emplace_back(std::vector<int>{6, 2, 7, 3});
    object.fFaces.emplace_back(std::vector<int>{0, 5, 1, 4});
    ENSURE(object.fFaces.size() == 6, "Rectangle was not initialized correctly");
    ENSURE(object.fPoints.size() == 8, "Rectangle was not initialized correctly");
    return object;
}

void Object::print(std::ofstream &ini, const int nr) {
    REQUIRE(properlyInitialized(), "Object was not initialized when calling print");
    REQUIRE(ini.is_open(), "Ofstream to ini is not open");
    REQUIRE(nr >= 0, "Nr must be greater than 0");
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

Object::Object() {
    _initCheck = this;
    ENSURE(this->properlyInitialized(), "Object was not initialized when constructed");
}

bool Object::properlyInitialized() const {
    return _initCheck == this;
}

std::ostream &operator<<(std::ostream &os, const Color &color) {
    REQUIRE(color.properlyInitialized(), "Color was not initialized when calling operator <<");
    os << "(" << color.fR << "," << color.fG << "," << color.fB << ")";
    return os;
}

bool Color::properlyInitialized() const {
    return _initCheck == this;
}

Color::Color(double fR, double fG, double fB) : fR(fR), fG(fG), fB(fB) {
    _initCheck = this;
    ENSURE(this->properlyInitialized(), "Color was not initialized when constructed");
}

Color::Color() {
    fR = 0;
    fG = 0;
    fB = 0;
    _initCheck = this;
    ENSURE(this->properlyInitialized(), "Color was not initialized when constructed");
}

void Color::set(const double &kR, const double &kG, const double &kB) {
    REQUIRE(properlyInitialized(), "Color was not initialized when calling set");
    fR = kR;
    fG = kG;
    fB = kB;
}

std::ostream &operator<<(std::ostream &os, const Pos &pos) {
    REQUIRE(pos.properlyInitialized(), "Position was not initialized when calling operator <<");
    os << "(" << pos.fX << "," << pos.fY << "," << pos.fZ << ")";
    return os;
}

bool Pos::properlyInitialized() const {
    return _initCheck == this;
}

Pos::Pos(double fX, double fY, double fZ) : fX(fX), fY(fY), fZ(fZ) {
    _initCheck = this;
    ENSURE(this->properlyInitialized(), "Pos was not initialized when constructed");
}

std::ostream &operator<<(std::ostream &os, const Face &face) {
    REQUIRE(face.properlyInitialized(), "Face was not initialized when calling operator <<");
    os << "(";
    for (unsigned int i = 0; i < face.fIndexes.size() - 1; ++i) {
        os << face.fIndexes[i] << ",";
    }
    os << face.fIndexes.back() << ")";
    return os;
}

Face::Face(std::vector<int> fIndexes) : fIndexes(std::move(fIndexes)) {
    _initCheck = this;
    ENSURE(this->properlyInitialized(), "Face was not initialized when constructed");
}

bool Face::properlyInitialized() const {
    return _initCheck == this;
}

// source: Serge Demeyer - TicTactToe in C++, Ansi-style
bool FileExists(const std::string &filename) {
    struct stat st{};
    if (stat(filename.c_str(), &st) != 0) return false;
    std::ifstream f(filename);
    if (f.good()) {
        f.close();
        return true;
    }
    f.close();
    return false;
}


