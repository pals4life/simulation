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
#include "../datatypes/Network.h"
#include "VAbstractParser.h"

class NetworkParser : public VAbstractParser {
public:

	/**
	 * 	ENSURE(this->properlyInitialized(), "NetworkParser was not initialized when constructed");
	 */
	NetworkParser();

	/**
	 * 	REQUIRE(this->properlyInitialized(), "NetworkParser was not initialized when calling parseNetwork");
	 *	REQUIRE(element, "Failed to parse network: no element");
	 *	ENSURE(fNetwork, "Failed to parse network: no network");
	 */
	Network *parseNetwork(TiXmlElement *element);

	/**
	 * 	REQUIRE(this->properlyInitialized(), "NetworkParser was not initialized when calling getNetwork");
	 *	ENSURE(fNetwork, "Failed to parse network: no network");
	 */
	Network *getNetwork() const;

	/**
	 * 	REQUIRE(a && b, "Failed to compare vehicle pointers: no vehicles");
	 */
	static bool compareVehiclePointers(const IVehicle *a, const IVehicle *b);

private:
	Network *fNetwork;
};


#endif //SIMULATION_NETWORKPARSER_H
