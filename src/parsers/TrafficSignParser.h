//============================================================================
// @name        : TrafficSignParser.h
// @author      : Ward Gauderis
// @date        : 4/21/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#ifndef SIMULATION_TRAFFICSIGNPARSER_H
#define SIMULATION_TRAFFICSIGNPARSER_H

#include "../datatypes/TrafficSigns.h"
#include "VAbstractParser.h"

enum TrafficSigns {
    trafficLight, busStop, zone, error
};

class TrafficSignParser : public VAbstractParser {
public:

    /**
     *  ENSURE(this->properlyInitialized(), "TrafficSignParser was not initialized when constructed");
     */
    TrafficSignParser();

    /**
     *  REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling parseTrafficSign");
     *  REQUIRE(element, "Failed to parse traffic sign: no element");
     *  ENSURE(fZone || fBusStop || fTrafficLight, "Failed to parse traffic sign: no traffic sign");
     */
    TrafficSigns parseTrafficSign(TiXmlElement *element);

    /**
     *  REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling getgetTrafficLightRoad");
     *  ENSURE(fTrafficLight, "Failed to parse traffic light: no traffic light");
     */
    TrafficLight *getTrafficLight() const;

    /**
     *  REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling getBusStop");
     *  ENSURE(fBusStop, "Failed to parse bus stop: no bus stop");
     */
    BusStop *getBusStop() const;

    /**
     *  REQUIRE(this->properlyInitialized(), "TrafficSignParser was not initialized when calling getZone");
     *  ENSURE(fZone, "Failed to parse zone: no zone");
     */
    Zone *getZone() const;

    std::string parseRoad(TiXmlElement *element);


private:
    TrafficLight *fTrafficLight;
    BusStop *fBusStop;
    Zone *fZone;

};


#endif //SIMULATION_TRAFFICSIGNPARSER_H
