//============================================================================
// @name        : NetworkExporterTester.cpp
// @author      : Thomas Dooms, Ward Gauderis
// @date        : 3/26/19
// @version     : 2.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : Tests for NetworkExporter
//============================================================================

#include "../exporters/NetworkExporter.h"
#include "../parsers/NetworkParser.h"
#include <gtest/gtest.h>
#include <fstream>
#include <stdlib.h>

class NetworkExporterTester : public ::testing::Test {
protected:
    friend class NetworkExporter;

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(NetworkExporterTester, Init) {
    EXPECT_DEATH(NetworkExporter::init(NULL, "test", "test"), "Failed to export network: no network");
    EXPECT_EQ(false, NetworkExporter::properlyInitialized());
    NetworkParser parser;
    parser.loadFile("inputfiles/testinputs/test1.xml");
//    Network *network = parser.parseNetwork(parser.getRoot());
//    network->startSimulation(NULL, "simple", "impression", true, false);
}

TEST_F(NetworkExporterTester, Finish) {
    EXPECT_DEATH(NetworkExporter::finish(), "NetworkExporter was not initialized when calling finish");
    EXPECT_EQ(false, NetworkExporter::properlyInitialized());
}

TEST_F(NetworkExporterTester, AddSection) {
    EXPECT_DEATH(NetworkExporter::addSection(NULL, 0), "NetworkExporter was not initialized when calling addSection");
    Network test({});
    testing::internal::CaptureStdout();
    NetworkExporter::init(&test, "test", "test");
    EXPECT_EQ("\n-------------------------------------------------\nOne character is 0.000000 meters",
              testing::internal::GetCapturedStdout());
    EXPECT_DEATH(NetworkExporter::addSection(NULL, 0), "Failed to add section: no network");
    EXPECT_EQ(true, NetworkExporter::properlyInitialized());
    int res = system("rm outputfiles/test.txt >/dev/null 2>&1");
    EXPECT_EQ(0, res % 256);
}

TEST_F(NetworkExporterTester, Whitespace) {
    EXPECT_EQ("     ", NetworkExporter::whitespace(5));
    EXPECT_EQ("", NetworkExporter::whitespace(-1));
    EXPECT_EQ("", NetworkExporter::whitespace(0));
}

TEST_F(NetworkExporterTester, PrintLane) {
    EXPECT_DEATH(NetworkExporter::printLane({}, 0, 0), "NetworkExporter was not initialized when calling printLane");
    Network test({});
    testing::internal::CaptureStdout();
    NetworkExporter::init(&test, "test", "test");
    EXPECT_EQ("\n-------------------------------------------------\nOne character is 0.000000 meters",
              testing::internal::GetCapturedStdout());
    testing::internal::CaptureStdout();
    NetworkExporter::printLane({}, 0, 0);
    EXPECT_EQ("", testing::internal::GetCapturedStdout());
    testing::internal::CaptureStdout();
    std::vector<std::vector<char>> lane;
    lane.resize(20);
    lane[0].push_back('A');
    lane[0].push_back('B');
    lane[1].push_back('C');
    NetworkExporter::printLane(lane, 2, 0);
    EXPECT_EQ("1 AC==================\n     B                   \n", testing::internal::GetCapturedStdout());
    int res = system("rm outputfiles/test.txt >/dev/null 2>&1");
    EXPECT_EQ(0, res % 256);
}

TEST_F(NetworkExporterTester, Tee) {
    EXPECT_DEATH(NetworkExporter::tee<std::string>("test", false),
                 "NetworkExporter was not initialized when calling tee");
    Network test({});
    testing::internal::CaptureStdout();
    NetworkExporter::init(&test, "test", "test");
    EXPECT_EQ("\n-------------------------------------------------\nOne character is 0.000000 meters",
              testing::internal::GetCapturedStdout());
    testing::internal::CaptureStdout();
    NetworkExporter::tee<std::string>("test", false);
    EXPECT_EQ("test", testing::internal::GetCapturedStdout());
    int res = system("rm outputfiles/test.txt >/dev/null 2>&1");
    EXPECT_EQ(0, res % 256);
}
