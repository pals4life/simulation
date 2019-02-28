//============================================================================
// @name        : NetworkParser.h
// @author      : Ward Gauderis
// @date        : 2/26/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Network from XML.
//============================================================================

#ifndef SIMULATION_NETWORKPARSER_H
#define SIMULATION_NETWORKPARSER_H


#include <string>
#include <vector>
#include "../Network.h"
#include "VAbstractParser.h"

class NetworkParser : public VAbstractParser {
public:
	NetworkParser();

	virtual ~NetworkParser();

	Network *parseNetwork(TiXmlElement *element);

	Network *getNetwork() const;

	static bool compareVehiclePointers(const IVehicle* a, const IVehicle* b);

private:
	Network *fNetwork;
};


#endif //SIMULATION_NETWORKPARSER_H
