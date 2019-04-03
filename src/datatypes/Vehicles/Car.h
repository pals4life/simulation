//============================================================================
// @name        : Car.h
// @author      : Thomas Dooms
// @date        : 26/02/2019
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : implementation of the vehicle interface
//============================================================================

#ifndef ITERATORS_CAR_H
#define ITERATORS_CAR_H


#include "Vehicle.h"

class Car : public Vehicle
{
public:
    /**
    * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
    * REQUIRE(position >= 0, "Position must be greater than 0");
    * REQUIRE(!license.empty(), "License plate must be valid");
    *
    * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
    */
    Car(const std::string& license, double pos, double velocity);

    /**
    * REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getVehicleLength");
    */
    virtual double getVehicleLength() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getType");
    */
    virtual std::string getType() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMaxSpeed");
    */
    virtual double getMaxSpeed() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMinSpeed");
    */
    virtual double getMinSpeed() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMaxAcceleration");
    */
    virtual double getMaxAcceleration() const;

    /**
    * REQUIRE(this->properlyInitialized(), "Car was not initialized when calling getMinAcceleration");
    */
    virtual double getMinAcceleration() const;

protected:
    static const double fgkMaxAcceleration;
    static const double fgkMinAcceleration;

    static const double fgkMinSpeed;
    static const double fgkMaxSpeed;

    static const double fgkVehicleLength;
};


#endif //ITERATORS_CAR_H
