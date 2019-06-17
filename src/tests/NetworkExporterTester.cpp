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
#include "Utils.h"

class NetworkExporterTester : public ::testing::Test {
protected:
    friend class NetworkExporter;

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(NetworkExporterTester, Init) {
    testing::internal::CaptureStdout();
    EXPECT_DEATH(NetworkExporter::init(NULL, "test", "test"), "Failed to export network: no network");
    EXPECT_EQ(false, NetworkExporter::properlyInitialized());
    NetworkParser parser;
    EXPECT_TRUE(parser.loadFile("inputfiles/testinputs/test1.xml"));
    Network *network = parser.parseNetwork(parser.getRoot());
    NetworkExporter::init(network, "testoutputs/NetworkExporterTester-Init(simple)",
                          "testoutputs/NetworkExporterTester-Init(impression)");
    NetworkExporter::finish();
    delete network;
    network = new Network({});
    NetworkExporter::init(network, "testoutputs/NetworkExporterTester-Init(impression)",
                          "testoutputs/NetworkExporterTester-Init(impression)");
    NetworkExporter::finish();
    delete network;
    testing::internal::GetCapturedStdout();
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-Init(impression).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-Init(impression)-expected.txt"));
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-Init(simple).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-Init(simple)-expected.txt"));
}

TEST_F(NetworkExporterTester, Finish) {
    EXPECT_DEATH(NetworkExporter::finish(), "NetworkExporter was not initialized when calling finish");
    EXPECT_EQ(false, NetworkExporter::properlyInitialized());
}

TEST_F(NetworkExporterTester, AddSection) {
    EXPECT_DEATH(NetworkExporter::addSection(NULL, 0), "NetworkExporter was not initialized when calling addSection");
    NetworkParser parser;
    EXPECT_TRUE(parser.loadFile("inputfiles/testinputs/test1.xml"));
    Network *network = parser.parseNetwork(parser.getRoot());
    testing::internal::CaptureStdout();
    NetworkExporter::init(network, "testoutputs/NetworkExporterTester-AddSection(simple)",
                          "testoutputs/NetworkExporterTester-AddSection(impression)");
    EXPECT_DEATH(NetworkExporter::addSection(NULL, 0), "Failed to add section: no network");
    EXPECT_EQ(true, NetworkExporter::properlyInitialized());
    NetworkExporter::addSection(network, 0);
    NetworkExporter::finish();
    testing::internal::GetCapturedStdout();
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-AddSection(impression).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-AddSection(impression)-expected.txt"));
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-AddSection(simple).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-AddSection(simple)-expected.txt"));
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
    NetworkExporter::init(&test, "testoutputs/NetworkExporterTester-PrintLane(simple)",
                          "testoutputs/NetworkExporterTester-PrintLane(impression)");
    NetworkExporter::printLane({}, 0, 0);
    std::vector<std::vector<char>> lane;
    lane.resize(20);
    lane[0].push_back('A');
    lane[0].push_back('B');
    lane[1].push_back('C');
    NetworkExporter::tee("\n | ", false);
    NetworkExporter::printLane(lane, 2, 0);
    NetworkExporter::printLane(lane, 2, 1);
    NetworkExporter::finish();
    testing::internal::GetCapturedStdout();
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-PrintLane(impression).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-PrintLane(impression)-expected.txt"));
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-PrintLane(simple).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-PrintLane(simple)-expected.txt"));
}

TEST_F(NetworkExporterTester, Tee) {
    EXPECT_DEATH(NetworkExporter::tee<std::string>("test", false),
                 "NetworkExporter was not initialized when calling tee");
    Network test({});
    testing::internal::CaptureStdout();
    NetworkExporter::init(&test, "test", "test");
    EXPECT_EQ("\n-------------------------------------------------\nOne character is 0.000000 meters\n",
              testing::internal::GetCapturedStdout());
    testing::internal::CaptureStdout();
    NetworkExporter::tee<std::string>("test", false);
    EXPECT_EQ("test", testing::internal::GetCapturedStdout());
    int res = system("rm outputfiles/test.txt >/dev/null 2>&1");
    EXPECT_EQ(0, res % 256);
}

TEST_F(NetworkExporterTester, CgExport) {
    EXPECT_DEATH(NetworkExporter::cgExport(NULL, 0),
                 "NetworkExporter was not initialized when calling cgExport");

    Network test({});
    testing::internal::CaptureStdout();
    NetworkExporter::init(&test, "test", "test");
    testing::internal::GetCapturedStdout();

    EXPECT_DEATH(NetworkExporter::cgExport(NULL, 0), "Failed to export to cg: no network");

    NetworkExporter::cgExport(&test, 0);
    NetworkExporter::finish();
    int res = system(
            "mv outputfiles/cg.ini \"outputfiles/testoutputs/NetworkExporterTester-CGExport(empty).txt\" >/dev/null 2>&1");
    EXPECT_EQ(0, res);
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-CGExport(empty).txt",
                            "outputfiles/testoutputs/NetworkExporterTester-CGExport(empty)-expected.txt"));

    NetworkParser parser;
    bool loaded = parser.loadFile("inputfiles/testinputs/test13.xml");
    EXPECT_TRUE(loaded);
    if (loaded) {
        testing::internal::CaptureStdout();
        Network *network = parser.parseNetwork(parser.getRoot());
        NetworkExporter::init(network, "test", "test");
        NetworkExporter::cgExport(network, 0);
        testing::internal::GetCapturedStdout();
        res = system(
                "mv outputfiles/cg.ini \"outputfiles/testoutputs/NetworkExporterTester-CGExport(full).txt\" >/dev/null 2>&1");
        EXPECT_EQ(0, res);
        res = system(
                "mv outputfiles/cg.bmp \"outputfiles/testoutputs/NetworkExporterTester-CGExport(full).bmp\" >/dev/null 2>&1");
        EXPECT_EQ(0, res);
        EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-CGExport(full).txt",
                                "outputfiles/testoutputs/NetworkExporterTester-CGExport(full)-expected.txt"));
        EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkExporterTester-CGExport(full).bmp",
                                "outputfiles/testoutputs/NetworkExporterTester-CGExport(full)-expected.bmp"));

        NetworkExporter::finish();
        delete network;
    }
    res = system("rm outputfiles/test.txt >/dev/null 2>&1");
    EXPECT_EQ(0, res % 256);
}
