#include "IVehicle.h"
#include <iostream>
#include "../tests/DesignByContract.h"

const double IVehicle::fgkMinVehicleDist = 5.0;

IVehicle::IVehicle(const std::string& license, double position, double velocity)
{
    REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    REQUIRE(position >= 0, "Position must be greater than 0");
    REQUIRE(!license.empty(), "License plate must be valid");

    _initCheck = this;
    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;

    ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
}

bool IVehicle::properlyInitialized() const
{
    return _initCheck == this;
}

std::string IVehicle::getLicensePlate()
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
    return fLicensePlate;
}

double IVehicle::getPosition() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
    return fPosition;
}

double IVehicle::getVelocity() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
    return fVelocity;
}

double IVehicle::getAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
    return fAcceleration;
}

double& IVehicle::getPosition()
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
    return fPosition;
}

double& IVehicle::getVelocity()
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
    return fVelocity;
}

double& IVehicle::getAcceleration()
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
    return fAcceleration;
}

bool& IVehicle::setMoved()
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
    return moved;
}

bool operator<(const IVehicle& a, const IVehicle& b)
{
    REQUIRE(a.properlyInitialized() && b.properlyInitialized(), "one of the Vehicles was not initialized when calling operator<");
    return a.fPosition > b.fPosition;
}

void IVehicle::printVehicle(std::ostream& stream, const std::string& roadName) const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling printVehicle");
    const std::string toString[1] = {"auto"};
    stream << "Voertuig: " + toString[getType()] +'('+ fLicensePlate + ")\n";
    stream << "  -> Baan    : " << roadName      << '\n';
    stream << "  -> Positie : " << fPosition     << '\n';
    stream << "  -> Snelheid: " << fVelocity*3.6 << '\n';
}

