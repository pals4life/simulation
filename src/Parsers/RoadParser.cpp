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
	froad = new Road;
}

RoadParser::~RoadParser() {

}

Road *RoadParser::parseRoad(TiXmlElement *const element) {
	const std::string kname = readElement(element, "naam");
	const int kmaxSpeed = std::atoi(readElement(element, "snelheidslimiet").c_str());
	const int klength = std::atoi(readElement(element, "lengte").c_str());
	const std::string Kconnection = readElement(element, "verbinding");
	return froad;
}

Road *RoadParser::getRoad() {
	return froad;
}
