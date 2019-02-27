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
	road = new Road;
}

RoadParser::~RoadParser() {

}

Road *RoadParser::parseRoad(TiXmlElement *const element) {
	return road;
}

Road *RoadParser::getRoad() {
	return road;
}
