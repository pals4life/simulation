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


#include <tinyxml.h>
#include "../Road.h"
#include "VAbstractParser.h"

class RoadParser: public VAbstractParser {
	Road *froad;
public:
	RoadParser();

	virtual ~RoadParser();

	Road *parseRoad(TiXmlElement *element);

	Road *getRoad();
};


#endif //SIMULATION_ROADPARSER_H
