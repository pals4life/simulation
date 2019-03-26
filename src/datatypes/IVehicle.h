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
    /**
     * REQUIRE(velocity >= 0, "Velocity must be greater than 0");
     * REQUIRE(position >= 0, "Position must be greater than 0");
     * REQUIRE(!license.empty(), "License plate must be valid");
     *
     * ENSURE(this->properlyInitialized(), "Vehicle constructor must end in properlyInitialized state");
     */
    IVehicle(const std::string& license, double pos, double velocity);
    virtual ~IVehicle(){}

    virtual void move(const IVehicle* next, double speedLimit, double offset = 0) = 0;

    virtual double getVehicleLength() const = 0;
    virtual EVehicleType getType() const = 0;

    virtual double getMaxVelocity() const = 0;
    virtual double getMinVelocity() const = 0;

    virtual double getMaxAcceleration() const = 0;
    virtual double getMinAcceleration() const = 0;

    bool properlyInitialized() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getLicensePlate");
     */
    std::string getLicensePlate();

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
     */
    double getPosition() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
     */
    double getVelocity() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
     */
    double getAcceleration() const;

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getPosition");
     */
    double& getPosition();

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getVelocity");
     */
    double& getVelocity();

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling getAcceleration");
     */
    double& getAcceleration();

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling setMoved");
     */
    bool& setMoved();

    /**
     * REQUIRE(a.properlyInitialized() && b.properlyInitialized(), "one of the Vehicles was not initialized when calling operator<");
     */
    friend bool operator<(const IVehicle& a, const IVehicle& b);

    /**
     * REQUIRE(this->properlyInitialized(), "Vehicle was not initialized when calling printVehicle");
     */
    void printVehicle(std::ostream& stream, const std::string& roadName) const;


protected:
    IVehicle* _initCheck;

    std::string fLicensePlate;
    bool moved;

    double fPosition;
    double fVelocity;
    double fAcceleration;

    static const double fgkMinVehicleDist;
};


#endif
