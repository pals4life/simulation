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
#include "../Road.h"
#include "VAbstractParser.h"

class RoadParser : public VAbstractParser {
public:
	RoadParser();

	Road *parseRoad(TiXmlElement *element);

	Road *getRoad() const;

	std::string parseConnection(TiXmlElement *element);

private:
	Road *fRoad;

	std::set<std::string> fNames;

};


#endif //SIMULATION_ROADPARSER_H
