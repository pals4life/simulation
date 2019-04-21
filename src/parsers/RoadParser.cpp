//============================================================================
// @name        : RoadParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Road from XML.
//============================================================================

#include <vector>
#include "RoadParser.h"
#include "../datatypes/DesignByContract.h"
#include <algorithm>
#include <iostream>

RoadParser::RoadParser() {
    fRoad = NULL;
    ENSURE(this->properlyInitialized(), "RoadParser was not initialized when constructed");
}


Road *RoadParser::parseRoad(TiXmlElement *const element) {
    REQUIRE(this->properlyInitialized(), "RoadParser was not initialized when calling parseRoad");
    REQUIRE(element, "Failed to parse road: no element");
    const std::string kName = readElement(element, "naam");
    if (kName.empty()) {
        std::cerr << "Failed to parse road with name " << kName
                  << ": name cannot be empty" << std::endl;
        return NULL;
    }
    if (!fNames.insert(kName).second) {
        std::cerr << "Failed to parse road with name " << kName
                  << ": name already in use" << std::endl;
        return NULL;
    }
    const std::string kMax = readElement(element, "snelheidslimiet");
    std::string::const_iterator it1 = kMax.begin();
    while (it1 != kMax.end() && std::isdigit(*it1)) ++it1;
    if (!(!kMax.empty() && it1 == kMax.end())) {
        std::cerr << "Failed to parse road with name " << kName
                  << ": max speed is no positive integer" << std::endl;
        return NULL;
    }
    const double kMaxSpeed = std::atof(kMax.c_str()) / 3.6;
    const std::string kLen = readElement(element, "lengte");
    std::string::const_iterator it2 = kLen.begin();
    while (it2 != kLen.end() && std::isdigit(*it2)) ++it2;
    if (!(!kLen.empty() && it2 == kLen.end())) {
        std::cerr << "Failed to parse road with name " << kName
                  << ": length is no positive integer" << std::endl;
        return NULL;
    }
    const double kLength = std::atof(kLen.c_str());
    const std::string kLan = readElement(element, "rijstroken");
    std::string::const_iterator it3 = kLan.begin();
    while (it3 != kLan.end() && std::isdigit(*it3)) ++it3;
    if (it3 != kLan.end()) {
        std::cerr << "Failed to parse road with name " << kName
                  << ": number of lanes is no positive integer" << std::endl;
        return NULL;
    }
    uint32_t lanes = std::atoi(kLan.c_str());
    if (kLan.empty()) {
        lanes = 1;
    }
    std::vector<Zone *> zones;
    zones.push_back(new Zone(0, kMaxSpeed));
    std::vector<BusStop *> stops;
    std::vector<TrafficLight *> lights;
    fRoad = new Road(kName, NULL, kLength, lanes, zones, stops, lights);
    ENSURE(fRoad, "Failed to parse road: no road");
    return fRoad;
}

Road *RoadParser::getRoad() const {
    REQUIRE(this->properlyInitialized(), "RoadParser was not initialized when calling getRoad");
    ENSURE(fRoad, "Failed to parse road: no road");
    return fRoad;
}

std::string RoadParser::parseConnection(TiXmlElement *element) {
    REQUIRE(this->properlyInitialized(), "RoadParser was not initialized when calling parseConnection");
    REQUIRE(element, "Failed to parse connection: no element");
    return readElement(element, "verbinding");
}
