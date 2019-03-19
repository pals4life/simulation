//============================================================================
// @name        : VehicleParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Vehicle from XML.
//============================================================================

#include <iostream>
#include <algorithm>
#include "VehicleParser.h"
#include "../datatypes/Car.h"
#include "../tests/DesignByContract.h"

VehicleParser::VehicleParser() {
	fVehicle = NULL;
	ENSURE(this->properlyInitialized(), "VehicleParser was not initialized when constructed");
}

IVehicle *VehicleParser::parseVehicle(TiXmlElement *const element) {
	REQUIRE(this->properlyInitialized(), "VehicleParser was not initialized when calling parseVehicle");
	REQUIRE(element, "Failed to parse vehicle: no element");
	const std::string kType = readElement(element, "type");
	if (kType == "AUTO") {
		const std::string kLicensePlate = readElement(element, "nummerplaat");
		if (kLicensePlate.empty()) {
			std::cerr << "Failed to parse vehicle with license plate " << kLicensePlate
					  << ": license plate cannot be empty" << std::endl;
			return NULL;
		}
		if (!fNames.insert(kLicensePlate).second) {
			std::cerr << "Failed to parse vehicle with license plate " << kLicensePlate
					  << ": license plate already in use" << std::endl;
			return NULL;
		}
		const std::string kPos = readElement(element, "positie");
		std::string::const_iterator it1 = kPos.begin();
		while (it1 != kPos.end() && std::isdigit(*it1)) ++it1;
		if (!(!kPos.empty() && it1 == kPos.end())) {
			std::cerr << "Failed to parse vehicle with license plate " << kLicensePlate
					  << ": position is no positive integer" << std::endl;
			return NULL;
		}
		const double kPosition = std::atof(kPos.c_str());
		const std::string kSpe = readElement(element, "snelheid");
		std::string::const_iterator it2 = kSpe.begin();
		while (it2 != kSpe.end() && std::isdigit(*it2)) ++it2;
		if (!(!kSpe.empty() && it2 == kSpe.end())) {
			std::cerr << "Failed to parse vehicle with license plate " << kLicensePlate
					  << ": speed is no positive integer" << std::endl;
			return NULL;
		}
		const double kSpeed = std::atof(kSpe.c_str());
		fVehicle = new Car(kLicensePlate, kPosition, kSpeed);
	} else {
		std::cerr << "Failed to parse vehicle of type " + kType + ": skipping element" << std::endl;
		return NULL;
	}
	ENSURE(fVehicle, "Failed to parse vehicle: no vehicle");
	return fVehicle;
}

IVehicle *VehicleParser::getVehicle() const {
	REQUIRE(this->properlyInitialized(), "VehicleParser was not initialized when calling getVehicle");
	ENSURE(fVehicle, "Failed to parse vehicle: no vehicle");
	return fVehicle;
}

std::string VehicleParser::parseRoad(TiXmlElement *const element) {
	REQUIRE(this->properlyInitialized(), "VehicleParser was not initialized when calling parseRoad");
	return readElement(element, "baan");
}
