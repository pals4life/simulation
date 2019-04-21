//============================================================================
// @name        : TrafficSignParser.cpp
// @author      : Ward Gauderis
// @date        : 4/21/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include <iostream>
#include "TrafficSignParser.h"
#include "../DesignByContract.h"

TrafficSignParser::TrafficSignParser() {
    fTrafficLight = NULL;
    fBusStop = NULL;
    fZone = NULL;
    ENSURE(this->properlyInitialized(), "TrafficSignParser was not initialized when constructed");
}

TrafficLight *TrafficSignParser::getTrafficLight() const {
    REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling getgetTrafficLightRoad");
    ENSURE(fTrafficLight, "Failed to parse traffic light: no traffic light");
    return fTrafficLight;
}

BusStop *TrafficSignParser::getBusStop() const {
    REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling getBusStop");
    ENSURE(fBusStop, "Failed to parse bus stop: no bus stop");
    return fBusStop;
}

Zone *TrafficSignParser::getZone() const {
    REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling getZone");
    ENSURE(fZone, "Failed to parse zone: no zone");
    return fZone;
}

TrafficSigns TrafficSignParser::parseTrafficSign(TiXmlElement *element) {
    REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling parseTrafficSign");
    REQUIRE(element, "Failed to parse traffic sign: no element");
    const std::string kType = readElement(element, "type");
    const std::string kPos = readElement(element, "positie");
    std::string::const_iterator it1 = kPos.begin();
    while (it1 != kPos.end() && std::isdigit(*it1)) ++it1;
    if (!(!kPos.empty() && it1 == kPos.end())) {
        std::cerr << "Failed to parse traffic sign: position is no positive integer" << std::endl;
        return error;
    }
    const double kPosition = std::atof(kPos.c_str());
    if (kType == "ZONE") {
        const std::string kMax = readElement(element, "snelheidslimiet");
        std::string::const_iterator it2 = kMax.begin();
        while (it2 != kMax.end() && std::isdigit(*it2)) ++it2;
        if (!(!kMax.empty() && it2 == kMax.end())) {
            std::cerr << "Failed to parse zone: max speed is no positive integer" << std::endl;
            return error;
        }
        const double kMaxSpeed = std::atof(kMax.c_str()) / 3.6;;
        fZone = new Zone(kPosition, kMaxSpeed);
        ENSURE(fZone, "Failed to parse traffic sign: no traffic sign");
        return zone;
    } else if (kType == "BUSHALTE") {
        fBusStop = new BusStop(kPosition);
        ENSURE(busStop, "Failed to parse traffic sign: no traffic sign");
        return busStop;
    } else if (kType == "VERKEERSLICHT") {
        fTrafficLight = new TrafficLight(kPosition);
        ENSURE(fTrafficLight, "Failed to parse traffic sign: no traffic sign");
        return trafficLight;
    } else {
        std::cerr << "Failed to parse traffic sign of type " + kType + ": skipping element" << std::endl;
        return error;
    }
}

std::string TrafficSignParser::parseRoad(TiXmlElement *element) {
    REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling parseRoad");
    REQUIRE(element, "Failed to parse road: no element");
    return readElement(element, "baan");
}
