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
#include <algorithm>
#include <map>
#include <iostream>

Network *NetworkParser::parseNetwork(TiXmlElement *const element) {
	RoadParser rp;
	VehicleParser vp;
	std::vector<Road *> roads;
	std::map<std::string, std::vector<IVehicle *> > tempRoads;
	for (TiXmlElement *elem = element->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
		const std::string kType = elem->Value();
		if (kType == "BAAN") {
			roads.push_back(rp.parseRoad(elem));
		} else if (kType == "VOERTUIG") {
			std::string roadName;
			IVehicle *v = vp.parseVehicle(elem, roadName);
			tempRoads[roadName].push_back(v);
		} else {
			std::cerr << "Failed to load file: Unknown element." << std::endl; //TODO exception handling
		}
	}
	for (std::map<std::string, std::vector<IVehicle *> >::iterator it1 = tempRoads.begin();
		 it1 != tempRoads.end(); it1++) {
		std::sort(it1->second.begin(), it1->second.end(), compareVehiclePointers);
		Road *foundRoad = *std::find(roads.begin(), roads.end(), it1->first);
		for (std::vector<IVehicle *>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			foundRoad->enqueue(*it2);
		}
	}
	fNetwork = new Network(roads);
	return fNetwork;
}

Network *NetworkParser::getNetwork() const {
	return fNetwork;
}

NetworkParser::NetworkParser() {
	fNetwork = NULL;
}

NetworkParser::~NetworkParser() {

}

bool NetworkParser::compareVehiclePointers(const IVehicle *a, const IVehicle *b) {
	return *a < *b;
}
