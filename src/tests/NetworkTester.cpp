//============================================================================
// @name        : NetworkTester.cpp
// @author      : Thomas Dooms
// @date        : 3/19/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================

#include <gtest/gtest.h>
#include "../datatypes/Network.h"
#include "../datatypes/vehicles/Car.h"

class NetworkTester : public ::testing::Test
{
protected:
    friend class Network;

    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST_F(NetworkTester, NetworkSimulation1)
{
    std::vector<Road*> roads;
    const Zone* zone = new Zone(0, 100);
    roads.push_back(new Road("E13", NULL, 150, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>()));
    Network network(roads);

    network.startSimulation(NULL);
    EXPECT_EQ(network.getTicksPassed(), 1);
}

TEST_F(NetworkTester, NetworkSimulation2)
{
    std::vector<Road*> roads;
    const Zone* zone = new Zone(0, 100);
    roads.push_back(new Road("E13", NULL, 150, 1, std::vector<const Zone*>(1, zone), std::vector<const BusStop*>(), std::vector<const TrafficLight*>()));
    roads[0]->enqueue(new Car("ABC", 0, 0));
    Network network(roads);

    network.startSimulation(NULL);
    EXPECT_GT(network.getTicksPassed(), 0);
    EXPECT_EQ(roads[0]->isEmpty(), true);
}
