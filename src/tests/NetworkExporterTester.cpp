//============================================================================
// @name        : NetworkExporterTester.cpp
// @author      : Thomas Dooms
// @date        : 3/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "../datatypes/NetworkExporter.h"
#include <gtest/gtest.h>
#include <fstream>

class NetworkExporterTester : public ::testing::Test
{
protected:
    friend class NetworkExporter;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(NetworkExporterTester, NetworkExporterAddSection)
{
    NetworkExporter exporter;
    EXPECT_DEATH(exporter.addSection(20), "NetworkExporter was not initialized when calling addSection");
}

TEST_F(NetworkExporterTester, NetworkExporterFinish)
{
    std::vector<Road*> roads;
    Network* network = new Network(roads);
    NetworkExporter exporter;
    exporter.initialize(network, "name");
    exporter.finish();

    std::fstream stream;
    stream.open("outputfiles/name.txt");
    EXPECT_EQ(stream.fail(), 0);
    system("rm outputfiles/name.txt >/dev/null 2>&1");
}