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

enum EVehicleType {kCar};

class IVehicle
{
public:
    IVehicle(const std::string& license, double pos);

    virtual void move(const IVehicle* next) = 0;

    virtual double getVehicleLength() const = 0;
    virtual EVehicleType getType() const = 0;

    virtual double getMaxVelocity() const = 0;
    virtual double getMinVelocity() const = 0;

    virtual double getMaxAcceleration() const = 0;
    virtual double getMinAcceleration() const = 0;


    double getPosition() const;
    double getVelocity() const;
    double getAcceleration() const;

    double& getPosition();
    double& getVelocity();
    double& getAcceleration();

protected:
    std::string kfLicensePlate;

    double fPosition;
    double fVelocity;
    double fAcceleration;
};


#endif
