//============================================================================
// @name        : Motorcycle.h
// @author      : Thomas Dooms
// @date        : 4/3/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef SIMULATION_MOTORCYCLE_H
#define SIMULATION_MOTORCYCLE_H


#include "Vehicle.h"

class Motorcycle : public Vehicle
{
public:
    /**
    * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    * REQUIRE(position >= 0, "Position must be greater than 0");
    * REQUIRE(!license.empty(), "License plate must be valid");
    *
    * ENSURE(this->properlyInitialized(), "Motorcycle constructor must end in properlyInitialized state");
    */
    Motorcycle(const std::string& license, double pos, double velocity);

    /**
    * REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getVehicleLength");
    */
    virtual double getVehicleLength() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getType");
    */
    virtual std::string getType() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMaxVelocity");
    */
    virtual double getMaxVelocity() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMinVelocity");
    */
    virtual double getMinVelocity() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMaxAcceleration");
    */
    virtual double getMaxAcceleration() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Motorcycle was not initialized when calling getMinAcceleration");
    */
    virtual double getMinAcceleration() const;

protected:
    static const double fgkMaxAcceleration;
    static const double fgkMinAcceleration;

    static const double fgkMinSpeed;
    static const double fgkMaxSpeed;

    static const double fgkVehicleLength;
};


#endif //SIMULATION_MOTORCYCLE_H
