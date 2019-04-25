//============================================================================
// @name        : VehicleExporter.h
// @author      : Thomas Dooms
// @date        : 4/25/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef SIMULATION_VEHICLEEXPORTER_H
#define SIMULATION_VEHICLEEXPORTER_H

#include <fstream>
#include "../datatypes/vehicles/IVehicle.h"
#include "../DesignByContract.h"


class VehicleExporter
{
public:
    static void init(const std::string& kPath);

    static void addSection(const IVehicle* kVehicle);

    static void finish();
private:
    static std::ofstream fgFile;
};


#endif //SIMULATION_VEHICLEEXPORTER_H
