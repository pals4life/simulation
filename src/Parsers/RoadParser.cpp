//============================================================================
// @name        : RoadParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Road from XML.
//============================================================================

#include "RoadParser.h"

RoadParser::RoadParser() {
	fRoad = NULL;
}

RoadParser::~RoadParser() {

}

Road *RoadParser::parseRoad(TiXmlElement *const element) {
	const std::string kName = readElement(element, "naam");
	const int kMaxSpeed = std::atoi(readElement(element, "snelheidslimiet").c_str());
	const double kLength = std::atof(readElement(element, "lengte").c_str());
	const std::string KConnection = readElement(element, "verbinding");
	fRoad = new Road(kName, NULL, kLength, kMaxSpeed);
	return fRoad;
}

Road *RoadParser::getRoad() const {
	return fRoad;
}
