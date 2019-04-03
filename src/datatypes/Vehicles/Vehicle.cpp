#include "Vehicle.h"
#include <iostream>
#include "../../tests/DesignByContract.h"

double clamp(double val, double min, double max){ return std::max(std::min(val, max), min); }

const double Vehicle::fgkMinVehicleDist = 5.0;

Vehicle::Vehicle(const std::string& license, double position, double velocity)
{
    REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    REQUIRE(position >= 0, "Position must be greater than 0");
    REQUIRE(!license.empty(), "License plate must be valid");

    _initCheck = this;
    fMoved = false;
    fLicensePlate = license;

    fPosition = position;
    fVelocity = velocity;
    fAcceleration = 0;

    ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
}

bool Vehicle::properlyInitialized() const
{
    return _initCheck == this;
}

void Vehicle::move(const Vehicle* const next, double speedLimit, double offset)
{
    REQUIRE(speedLimit > 0, "Speedlimit must be greater than 0");
    REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    REQUIRE(next == NULL or next->getPosition() + offset - this->getPosition() > fgkMinVehicleDist, "distance between vehicles must be greater than minVehicleDist");

    if(fMoved) return;
    if(next != NULL and fPosition + fVelocity >= offset + next->getPosition() + next->getVelocity() - next->getVehicleLength()) std::cerr << "car crash is imminent.\n";

    fPosition += fVelocity;                                                                 // Calculate new positions
    fVelocity += fAcceleration;                                                             // Calculate new velocity

    const double kAcceleration = getFollowingAcceleration(next, offset);                    // calculate the following speed
    const std::pair<double, double> kMinMax = getMinMaxAcceleration(speedLimit);            // calculate the min and max acceleration possible
    fAcceleration = clamp(kAcceleration, kMinMax.first, kMinMax.second);                    // clamp the values

    fMoved = true;

    ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMaxAcceleration()), "Acceleration is too high / low");
}

double Vehicle::getFollowingAcceleration(const Vehicle* next, double offset) const
{
    if(next == NULL) return getMaxAcceleration();                                       // if there is not car in front, acceleration = max

    double ideal  = 0.75 * fVelocity + next->getVehicleLength() + 2;                    // ideal following distance = 3/4 speed + 2 meters extra
    double actual = offset+next->getPosition() - next->getVehicleLength() - fPosition;  // distance between 2 vehicles
    return 0.5 * (actual - ideal);                                                      // take the average
}

std::pair<double, double> Vehicle::getMinMaxAcceleration(double speedlimit) const
{
    double maxSpeed = std::min(speedlimit, getMaxSpeed());                              // take the maxSpeed as the minimum of both
    double minSpeed = std::max(0.0       , getMinSpeed());                              // if the minimum speed is negative for some reason

    double maxAcceleration = maxSpeed - getVelocity();                                  // check if going to fast
    double minAcceleration = minSpeed - getVelocity();                                  // check if going too slow

    double clampedMax = clamp(maxAcceleration, getMinAcceleration(), getMaxAcceleration());
    double clampedMin = clamp(minAcceleration, getMinAcceleration(), getMaxAcceleration());

    return std::pair<double, double>(clampedMin, clampedMax);
}

std::string Vehicle::getLicensePlate() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
    return fLicensePlate;
}

double Vehicle::getPosition() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
    return fPosition;
}
void Vehicle::setPosition(double position)
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setPosition");
    fPosition = position;
}

double Vehicle::getVelocity() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
    return fVelocity;
}

double Vehicle::getAcceleration() const
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
    return fAcceleration;
}

void Vehicle::setMoved(bool moved)
{
    REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
    fMoved = moved;
}

bool operator<(const Vehicle& a, const Vehicle& b)
{
    REQUIRE(a.properlyInitialized() && b.properlyInitialized(), "one of the Vehicles was not initialized when calling operator<");
    return a.fPosition > b.fPosition;
}

