//============================================================================
// @name        : IVehicle.cpp
// @author      : Thomas Dooms
// @date        : 26/02/2019
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : pure virtual class describing a vehicle
//============================================================================

#ifndef IVEHICLE_H
#define IVEHICLE_H

#include <string>
#include "../TrafficSigns.h"

class IVehicle
{
public:
    /**
     * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
     * REQUIRE(position >= 0, "Position must be greater than 0");
     * REQUIRE(!license.empty(), "License plate must be valid");
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
    IVehicle(const std::string& license, double pos, double velocity);
    virtual ~IVehicle(){}

    virtual double getVehicleLength() const = 0;
    virtual std::string getType() const = 0;

    virtual double getMaxSpeed() const = 0;
    virtual double getMinSpeed() const = 0;

    virtual double getMaxAcceleration() const = 0;
    virtual double getMinAcceleration() const = 0;

    bool properlyInitialized() const;

    /**
    * REQUIRE(speedLimit > 0, "Speedlimit must be greater than 0");
    * REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    * REQUIRE(next == NULL or next->getPosition() + offset - this->getPosition() > getMinVehicleDist(), "distance between vehicles must be greater than minVehicleDist");
    *
    * ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    * ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMinAcceleration()), "Acceleration is too high / low");
    */
    void move(std::pair<const IVehicle*, double> nextVehicle, std::pair<const TrafficLight*, double> nextTrafficLight, std::pair<const BusStop*, double> nextBusStop, double speedLimit);

    double getFollowingAcceleration(std::pair<const IVehicle*, double> nextVehicle) const;

    std::pair<double, double> getMinMaxAcceleration(double speedlimit) const;

    std::pair<bool, double> checkTrafficLights(std::pair<const TrafficLight*, double> nextTrafficLight) const;

    std::pair<bool, double> checkBusStop(std::pair<const BusStop*, double> nextBusStop) const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
     */
    std::string getLicensePlate()const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
     */
    double getPosition() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setPosition");
     */
    void setPosition(double position);

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
     */
    double getVelocity() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
     */
    double getAcceleration() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
     */
    void setMoved(bool moved);

    /**
     * REQUIRE(a.properlyInitialized() && b.properlyInitialized(), "one of the Vehicles was not initialized when calling operator<");
     */
    friend bool operator<(const IVehicle& a, const IVehicle& b);

protected:
    IVehicle* _initCheck;

    std::string fLicensePlate;
    bool fMoved;

    double fPosition;
    double fVelocity;
    double fAcceleration;

    static const double fgkMinVehicleDist;
};


#endif
