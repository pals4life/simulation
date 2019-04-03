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
#include "../datatypes/Vehicles/Car.h"

class NetworkTester : public ::testing::Test
{
protected:
    friend class Network;

    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST_F(NetworkTester, NetworkInit1)
{
    std::vector<Road*> roads;
    Network network(roads);

    EXPECT_DEATH(network.startSimulation(-1), "Amount of ticks must be a positive integer");
}

TEST_F(NetworkTester, NetworkSimulation1)
{
    std::vector<Road*> roads;
    roads.push_back(new Road("E123", NULL, 5000, 100));
    Network network(roads);

    network.startSimulation();
    EXPECT_EQ(network.getTicksPassed(), 0);
}

TEST_F(NetworkTester, NetworkSimulation2)
{
    std::vector<Road*> roads;
    roads.push_back(new Road("E123", NULL, 5000, 100));
    roads[0]->enqueue(new Car("ABC", 0, 0));
    Network network(roads);

    network.startSimulation();
    EXPECT_GT(network.getTicksPassed(), 0);
    EXPECT_EQ(roads[0]->isEmpty(), true);
}
