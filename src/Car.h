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


#include "IVehicle.h"

class Car : public IVehicle
{
public:
    Car(const std::string& license, double pos, double velocity);

    virtual void move(const IVehicle* next);

    virtual double getVehicleLength() const;
    virtual EVehicleType getType() const;

    virtual double getMaxVelocity() const;
    virtual double getMinVelocity() const;

    virtual double getMaxAcceleration() const;
    virtual double getMinAcceleration() const;

protected:
    static const double fgkMaxAcceleration;
    static const double fgkMinAcceleration;

    static const double fgkMinSpeed;
    static const double fgkMaxSpeed;

    static const double fgkVehicleLength;
};


#endif //ITERATORS_CAR_H
