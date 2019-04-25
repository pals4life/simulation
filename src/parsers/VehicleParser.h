//============================================================================
// @name        : VehicleParser.h
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Vehicle from XML.
//============================================================================

#ifndef SIMULATION_VEHICLEPARSER_H
#define SIMULATION_VEHICLEPARSER_H

#include <set>
#include "tinyxml/tinyxml.h"
#include "../datatypes/vehicles/IVehicle.h"
#include "VAbstractParser.h"

class VehicleParser : public VAbstractParser {
public:

	/**
	 * 	ENSURE(this->properlyInitialized(), "VehicleParser was not initialized when constructed");
	 */
	VehicleParser();

	/**
	 * 	REQUIRE(this->properlyInitialized(), "VehicleParser was not initialized when calling parseVehicle");
	 *	REQUIRE(element, "Failed to parse vehicle: no element");
	 *	ENSURE(fVehicle, "Failed to parse vehicle: no vehicle");
	 */
    IVehicle *parseVehicle(TiXmlElement *element);

	/**
	 * 	REQUIRE(this->properlyInitialized(), "VehicleParser was not initialized when calling getVehicle");
	 *	ENSURE(fVehicle, "Failed to parse vehicle: no vehicle");
	 */
    IVehicle *getVehicle() const;

	/**
	 * 	REQUIRE(this->properlyInitialized(), "VehicleParser was not initialized when calling parseRoad");
	 *  REQUIRE(element, "Failed to parse road: no element");
	 */
	std::string parseRoad(TiXmlElement *element);

private:
    IVehicle *fVehicle;

	std::set<std::string> fNames;
};


#endif //SIMULATION_VEHICLEPARSER_H
