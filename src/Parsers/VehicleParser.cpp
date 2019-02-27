//============================================================================
// @name        : VehicleParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Vehicle from XML.
//============================================================================

#include "VehicleParser.h"

VehicleParser::VehicleParser() {
	vehicle = NULL;
}

VehicleParser::~VehicleParser() {

}

IVehicle *VehicleParser::parseVehicle(TiXmlElement *const element) {

	return vehicle;
}

IVehicle *VehicleParser::getVehicle() {
	return vehicle;
}
