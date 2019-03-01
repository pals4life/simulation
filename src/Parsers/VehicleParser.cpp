//============================================================================
// @name        : VehicleParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Vehicle from XML.
//============================================================================

#include <iostream>
#include "VehicleParser.h"
#include "../Car.h"

VehicleParser::VehicleParser() {
	fVehicle = NULL;
}

VehicleParser::~VehicleParser() {

}

IVehicle *VehicleParser::parseVehicle(TiXmlElement *const element) {
	const std::string kType = readElement(element, "type");
	if (kType == "AUTO") {
		const std::string kLicensePlate = readElement(element, "nummerplaat");
		const double kPosition = std::atof(readElement(element, "positie").c_str());
		const double kSpeed = std::atof(readElement(element, "snelheid").c_str());
		fVehicle = new Car(kLicensePlate, kPosition, kSpeed);
	} else {
		std::cerr << "Failed to load file: Vehicle type not recognised." << std::endl; //TODO exception handling
	}
	return fVehicle;
}

IVehicle *VehicleParser::getVehicle() const {
	return fVehicle;
}

std::string VehicleParser::parseRoad(TiXmlElement *const element) {
	return readElement(element, "baan");
}
