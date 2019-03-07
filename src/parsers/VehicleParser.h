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

#include "tinyxml/tinyxml.h"
#include "../IVehicle.h"
#include "VAbstractParser.h"

class VehicleParser : public VAbstractParser {
public:
	VehicleParser();

	virtual ~VehicleParser();

	IVehicle *parseVehicle(TiXmlElement *element);

	IVehicle *getVehicle() const;

	std::string parseRoad(TiXmlElement *element);

private:
	IVehicle *fVehicle;

};


#endif //SIMULATION_VEHICLEPARSER_H
