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
#include <deque>

class Road;

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

    bool properlyInitialized() const;

    virtual double getVehicleLength() const = 0;
    virtual std::string getType() const = 0;

    virtual double getMaxSpeed() const = 0;
    virtual double getMinSpeed() const = 0;

    virtual double getMaxAcceleration() const = 0;
    virtual double getMinAcceleration() const = 0;

    /**
    * REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    *
    * ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    * ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMinAcceleration()), "Acceleration is too high / low");
    * ENSURE(nextVehicle.first == NULL or pairPosition<IVehicle>(nextVehicle) - getPosition() > getMinVehicleDist(), "distance between vehicles must be greater than minVehicleDist");
    * ENSURE(fPrevAcceleration.size() == 5, "Previous acceleration must contain 5 elements");
    */
    void move(uint32_t lane, uint32_t index, Road* road);

    //--------------------------------------------------------------------------------------------------//

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
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
     */
    double getMinVehicleDist() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
     */
    void setMoved(bool moved) const;

private:

    double getFollowingAcceleration(std::pair<const IVehicle*, double> nextVehicle) const;

    std::pair<double, double> getMinMaxAcceleration(double speedlimit) const;

    std::pair<bool, double> checkTrafficLights(std::pair<const TrafficLight*, double> nextTrafficLight) const;

    std::pair<bool, double> checkBusStop(std::pair<const BusStop*, double> nextBusStop) const;

    void checkLaneChange(bool trafficLight, uint32_t lane, uint32_t index, Road* road, bool left);

protected:
    IVehicle* _initCheck;

    std::string fLicensePlate;
    mutable bool fMoved;

    double fPosition;
    double fVelocity;
    double fAcceleration;

    std::deque<double> fPrevAcceleration;

    static const double fgkMinVehicleDist;
    static const double fgkEpsilonThreshold;
};


#endif
