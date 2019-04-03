//============================================================================
// @name        : Bus.h
// @author      : Thomas Dooms
// @date        : 4/3/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef SIMULATION_BUS_H
#define SIMULATION_BUS_H

#include "Vehicle.h"

class Bus : public Vehicle
{
public:
    /**
    * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    * REQUIRE(position >= 0, "Position must be greater than 0");
    * REQUIRE(!license.empty(), "License plate must be valid");
    *
    * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
    */
    Bus(const std::string& license, double pos, double velocity);

    /**
    * REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getVehicleLength");
    */
    virtual double getVehicleLength() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getType");
    */
    virtual std::string getType() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMaxSpeed");
    */
    virtual double getMaxSpeed() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMinSpeed");
    */
    virtual double getMinSpeed() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMaxAcceleration");
    */
    virtual double getMaxAcceleration() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Bus was not initialized when calling getMinAcceleration");
    */
    virtual double getMinAcceleration() const;

protected:
    static const double fgkMaxAcceleration;
    static const double fgkMinAcceleration;

    static const double fgkMinSpeed;
    static const double fgkMaxSpeed;

    static const double fgkVehicleLength;
};


#endif //SIMULATION_BUS_H
