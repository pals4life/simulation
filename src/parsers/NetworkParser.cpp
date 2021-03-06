//============================================================================
// @name        : NetworkParser.cpp
// @author      : Ward Gauderis
// @date        : 2/26/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Network from XML.
//============================================================================

#include "NetworkParser.h"
#include "TrafficSignParser.h"
#include "RoadParser.h"
#include "VehicleParser.h"
#include "../DesignByContract.h"
#include <algorithm>
#include <map>
#include <iostream>
#include <stdint.h>
#include "../datatypes/util.h"

Network *NetworkParser::parseNetwork(TiXmlElement *const element) {
    REQUIRE(this->properlyInitialized(), "NetworkParser was not initialized when calling parseNetwork");
    REQUIRE(element, "Failed to parse network: no element");
    RoadParser rp;
    VehicleParser vp;
    TrafficSignParser tp;
    std::vector<Road *> roads;
    std::map<std::string, std::vector<IVehicle *> > tempRoads;
    for (TiXmlElement *elem = element->FirstChildElement();
         elem != NULL; elem = elem->NextSiblingElement()) { //lees roads in
        const std::string kType = elem->Value();
        if (kType == "BAAN") {
            Road *road = rp.parseRoad(elem);
            if (road) {
                roads.push_back(road);
            }
        }
    }
    uint32_t roadNr = 0;
    for (TiXmlElement *elem = element->FirstChildElement();
         elem != NULL; elem = elem->NextSiblingElement()) { //lees verbindingen en voertuigen in
        const std::string kType = elem->Value();
        if (kType == "BAAN") {
            const std::string kConnection = rp.parseConnection(elem);
            if (!kConnection.empty()) {
                std::vector<Road *>::iterator found = std::find_if(roads.begin(), roads.end(), Comparator(kConnection));
                if (found == roads.end()) {
                    std::cerr << "Inconsistent traffic situation: road " << kConnection << " does not exist"
                              << std::endl;
                    roads[roadNr++]->setNextRoad(NULL);
                } else {
                    roads[roadNr++]->setNextRoad(*found);
                }
            }
        } else if (kType == "VOERTUIG") {
            IVehicle *vehicle = vp.parseVehicle(elem);
            if (vehicle) {
                tempRoads[vp.parseRoad(elem)].push_back(vehicle);
            }
        } else if (kType == "VERKEERSTEKEN") {
            const ETrafficSigns kSign = tp.parseTrafficSign(elem);
            if (kSign != kError) {
                std::vector<Road *>::iterator found = std::find_if(roads.begin(), roads.end(),
                                                                   Comparator(tp.parseRoad(elem)));
                if (found != roads.end()) {
                    Road *foundRoad = *found;
                    switch (kSign) {
                        case kTrafficLight:
                            foundRoad->addTrafficLight(tp.getTrafficLight());
                            break;
                        case kBusStop:
                            foundRoad->addBusStop(tp.getBusStop());
                            break;
                        case kZone:
                            foundRoad->addZone(tp.getZone());
                            break;
                        default:
                            break;
                    }
                } else {
                    std::cerr << "Inconsistent traffic situation: road " << tp.parseRoad(elem) << " does not exist"
                              << std::endl;
                }
            }
        } else {
            std::cerr << "Failed to recognize element " + kType + ": skipping element" << std::endl;
        }
    }
    for (std::map<std::string, std::vector<IVehicle *> >::iterator it1 = tempRoads.begin(); //check voor inconsistente verkeersituaties en plaats de auto's op de wegen
         it1 != tempRoads.end(); it1++) {
        std::sort(it1->second.rbegin(), it1->second.rend(), comparePositions<IVehicle>);
        std::vector<Road *>::iterator found = std::find_if(roads.begin(), roads.end(), Comparator(it1->first));
        if (found != roads.end()) {
            Road *foundRoad = *found;
            for (std::vector<IVehicle *>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                if ((*it2)->getPosition() >= foundRoad->getRoadLength()) {
                    std::cerr << "Inconsistent traffic situation: vehicle " << (*it2)->getLicensePlate()
                              << " is not on road "
                              << it1->first << std::endl;
                }
                std::vector<IVehicle *>::iterator it3 = it2;
                if (++it3 < it1->second.end() && abs((*it2)->getPosition() - (*it3)->getPosition()) < 5) {
                    std::cerr << "Inconsistent traffic situation: vehicle " << (*it2)->getLicensePlate()
                              << " is less than 5m away from vehicle " << (*it3)->getLicensePlate() << " on road "
                              << it1->first << std::endl;
                }
                const double kFromEnd = foundRoad->getRoadLength() - (*it2)->getPosition();
                if (kFromEnd < 5) {
                    if (Road *nextRoad = foundRoad->getNextRoad()) {
                        IVehicle *nextVehicle = NULL;
                        if (!tempRoads[nextRoad->getName()].empty()) {
                            nextVehicle = tempRoads[nextRoad->getName()].front();
                            if (nextVehicle->getPosition() + kFromEnd < 5) {
                                std::cerr << "Inconsistent traffic situation: vehicle " << (*it2)->getLicensePlate()
                                          << " is less than 5m away from vehicle " << nextVehicle->getLicensePlate()
                                          << " on roads " << it1->first << " and " << nextRoad->getName()
                                          << std::endl;
                            }
                        }
                    }
                }
                foundRoad->enqueue(*it2);
            }
        } else {
            std::cerr << "Inconsistent traffic situation: road " << it1->first << " does not exist" << std::endl;
        }
    }
    fNetwork = new Network(roads);
    ENSURE(fNetwork, "Failed to parse network: no network");
    return fNetwork;
}

Network *NetworkParser::getNetwork() const {
    REQUIRE(this->properlyInitialized(), "NetworkParser was not initialized when calling getNetwork");
    ENSURE(fNetwork, "Failed to parse network: no network");
    return fNetwork;
}

NetworkParser::NetworkParser() {
    fNetwork = NULL;
    ENSURE(this->properlyInitialized(), "NetworkParser was not initialized when constructed");
}