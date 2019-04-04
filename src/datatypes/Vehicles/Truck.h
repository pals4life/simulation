//============================================================================
// @name        : Truck.h
// @author      : Thomas Dooms
// @date        : 4/3/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef SIMULATION_TRUCK_H
#define SIMULATION_TRUCK_H

#include "IVehicle.h"

class Truck : public IVehicle
{
public:
    /**
    * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    * REQUIRE(position >= 0, "Position must be greater than 0");
    * REQUIRE(!license.empty(), "License plate must be valid");
    *
    * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
    */
    Truck(const std::string& license, double pos, double velocity);

    /**
    * REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getVehicleLength");
    */
    virtual double getVehicleLength() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getType");
    */
    virtual std::string getType() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMaxSpeed");
    */
    virtual double getMaxSpeed() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMinSpeed");
    */
    virtual double getMinSpeed() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMaxAcceleration");
    */
    virtual double getMaxAcceleration() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Truck was not initialized when calling getMinAcceleration");
    */
    virtual double getMinAcceleration() const;

protected:
    static const double fgkMaxAcceleration;
    static const double fgkMinAcceleration;

    static const double fgkMinSpeed;
    static const double fgkMaxSpeed;

    static const double fgkVehicleLength;
};


#endif //SIMULATION_TRUCK_H
