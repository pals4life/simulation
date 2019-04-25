//============================================================================
// @name        : VehicleExporter.cpp
// @author      : Thomas Dooms
// @date        : 4/25/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "VehicleExporter.h"

std::ofstream VehicleExporter::fgFile;

void VehicleExporter::init(const std::string& kPath)
{
    int res = system("mkdir outputfiles >/dev/null 2>&1");
    ENSURE(res == 0 or res == 256, "could not make directory");
    fgFile.open(("outputfiles/" + kPath + ".txt").c_str());
    ENSURE(fgFile.is_open(), "output file is not open");
}

void VehicleExporter::addSection(const IVehicle* kVehicle)
{
    const std::tuple<uint32_t, uint32_t, double, double> statistics = kVehicle->getStatistics();
    fgFile << "Voertuig: (" + kVehicle->getLicensePlate() + ")\n";
    fgFile << "  -> aantal seconden in simulatie: " << std::get<0>(statistics)                                          << " seconden\n";
    fgFile << "  -> aantal seconden stilstaan   : " << std::get<0>(statistics) - std::get<1>(statistics)                << " seconden\n";
    fgFile << "  -> aantal seconden rijden      : " << std::get<1>(statistics)                                          << " seconden\n";
    fgFile << "  -> gemmidelde snelheid         : " << std::get<2>(statistics) / double(std::get<0>(statistics)) * 3.6  << " km/h\n";
    fgFile << "  -> maximale snelheid           : " << std::get<3>(statistics)*3.6                                      << " km/h\n";
    fgFile << "  -> afgelegde weg               : " << std::get<2>(statistics)                                          << " m\n\n";
}

void VehicleExporter::finish()
{
    fgFile << std::flush;
    fgFile.close();
}