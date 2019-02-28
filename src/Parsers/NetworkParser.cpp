//============================================================================
// @name        : NetworkParser.cpp
// @author      : Ward Gauderis
// @date        : 2/26/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Network from XML.
//============================================================================

#include "NetworkParser.h"
#include "RoadParser.h"
#include "VehicleParser.h"
#include <tinyxml.h>

Network *NetworkParser::parseNetwork(TiXmlElement *const element) {
	RoadParser rp;
	VehicleParser vp;
	for (TiXmlElement *elem = element->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
		if (elem->ToText() == "BAAN") {
			Road* r = rp.parseRoad(elem);
		} else if (elem->ToText() == "VOERTUIG") {
			IVehicle* v = vp.parseVehicle(elem);
		} else {
			std::cerr << "Failed to load file: Unknown element." << std::endl; //TODO exception handling
		}
	}
	return fnetwork;
}

Network *NetworkParser::getNetwork() {
	return fnetwork;
}

NetworkParser::NetworkParser() {
	fnetwork = new Network;
}

NetworkParser::~NetworkParser() {

}
