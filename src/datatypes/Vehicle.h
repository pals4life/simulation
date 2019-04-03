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

class Vehicle
{
public:
    /**
     * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
     * REQUIRE(position >= 0, "Position must be greater than 0");
     * REQUIRE(!license.empty(), "License plate must be valid");
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
    Vehicle(const std::string& license, double pos, double velocity);
    virtual ~Vehicle(){}

    virtual double getVehicleLength() const = 0;
    virtual std::string getType() const = 0;

    virtual double getMaxVelocity() const = 0;
    virtual double getMinVelocity() const = 0;

    virtual double getMaxAcceleration() const = 0;
    virtual double getMinAcceleration() const = 0;

    bool properlyInitialized() const;

    /**
    * REQUIRE(speedLimit > 0, "Speedlimit must be greater than 0");
    * REQUIRE(this->properlyInitialized(), "moved vehicle must be properly initialized");
    *
    * ENSURE(getVelocity() >= 0, "Velocity cannot be negative");
    * ENSURE((getAcceleration() >= getMinAcceleration()) && (getAcceleration() <= getMinAcceleration()), "Acceleration is too high / low");
    */
    void move(const Vehicle* next, double speedLimit, double offset = 0);

    double getFollowingAcceleration(const Vehicle* next, double offset = 0) const;

    std::pair<double, double> getMinMaxAcceleration(double speedlimit) const;

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
    friend bool operator<(const Vehicle& a, const Vehicle& b);

protected:
    Vehicle* _initCheck;

    std::string fLicensePlate;
    bool fMoved;

    double fPosition;
    double fVelocity;
    double fAcceleration;

    static const double fgkMinVehicleDist;
};


#endif
