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
#include <algorithm>
#include <iostream>

RoadParser::RoadParser() {
	fRoad = NULL;
}

RoadParser::~RoadParser() {

}

Road *RoadParser::parseRoad(TiXmlElement *const element, const std::vector<Road *> &kRoads) {
	const std::string kName = readElement(element, "naam");
	const int kMaxSpeed = std::atoi(readElement(element, "snelheidslimiet").c_str());
	const double kLength = std::atof(readElement(element, "lengte").c_str());
	const std::string KConnection = readElement(element, "verbinding");
	Road *connection = NULL;
	if (!KConnection.empty()) {
		for (long unsigned int i = 0; i < kRoads.size(); ++i) {
			if (kRoads[i]->getName() == KConnection) {
				connection = kRoads[i];
				fRoad = new Road(kName, connection, kLength, kMaxSpeed);
				return fRoad;
			}
		}
	}
	fRoad = new Road(kName, connection, kLength, kMaxSpeed);
	return fRoad;
}

Road *RoadParser::getRoad() const {
	return fRoad;
}
