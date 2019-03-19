//============================================================================
// @name        : RoadParser.h
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Road from XML.
//============================================================================

#ifndef SIMULATION_ROADPARSER_H
#define SIMULATION_ROADPARSER_H


#include <set>
#include "tinyxml/tinyxml.h"
#include "../datatypes/Road.h"
#include "VAbstractParser.h"

class RoadParser : public VAbstractParser {
public:

	/**
	 * 	ENSURE(this->properlyInitialized(), "RoadParser was not initialized when constructed");
	 */
	RoadParser();

	/**
	 * 	REQUIRE(this->properlyInitialized(), "RoadParser was not initialized when calling parseRoad");
	 *	REQUIRE(element, "Failed to parse road: no element");
	 *	ENSURE(fRoad, "Failed to parse road: no road");
	 */
	Road *parseRoad(TiXmlElement *element);

	/**
	 * 	REQUIRE(this->properlyInitialized(), "RoadParser was not initialized when calling getRoad");
	 *	ENSURE(fRoad, "Failed to parse road: no road");
	 */
	Road *getRoad() const;

	/**
	 * 	REQUIRE(this->properlyInitialized(), "RoadParser was not initialized when calling parseConnection");
	 */
	std::string parseConnection(TiXmlElement *element);

private:
	Road *fRoad;

	std::set<std::string> fNames;

};


#endif //SIMULATION_ROADPARSER_H
