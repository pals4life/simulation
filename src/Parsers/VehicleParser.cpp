//============================================================================
// @name        : VehicleParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Vehicle from XML.
//============================================================================

#include "VehicleParser.h"
#include "../Car.h"

VehicleParser::VehicleParser() {
	fvehicle = NULL;
}

VehicleParser::~VehicleParser() {

}

IVehicle *VehicleParser::parseVehicle(TiXmlElement *const element) {
	const std::string ktype = readElement(element, "type");
	if (ktype == "AUTO") {
		fvehicle = new Car;
		const std::string klicensePlate = readElement(element, "nummerplaat");
		const std::string kroad = readElement(element, "baan");
		const int kposition = std::atoi(readElement(element, "positie").c_str());
		const int kspeed = std::atoi(readElement(element, "snelheid").c_str());
	} else {
		std::cerr << "Failed to load file: Vehicle type not recognised." << std::endl; //TODO exception handling
	}
	return fvehicle;
}

IVehicle *VehicleParser::getVehicle() {
	return fvehicle;
}
