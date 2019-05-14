//============================================================================
// @name        : VehicleParserTester.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for VehicleParser.
//============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include "Utils.h"
#include "../parsers/VehicleParser.h"

class VehicleParserTester : public ::testing::Test {
protected:
    friend class VehicleParser;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(VehicleParserTester, Init) {
    VehicleParser parser;
    EXPECT_DEATH(parser.getVehicle(), "Failed to parse vehicle: no vehicle");
}

TEST_F(VehicleParserTester, ParseVehicle) {
    VehicleParser parser;
    testing::internal::CaptureStderr();
    bool loaded = parser.loadFile("inputfiles/testinputs/test3.xml");
    testing::internal::GetCapturedStderr();
    EXPECT_EQ(true, loaded);
    if (loaded) {
        EXPECT_DEATH(parser.parseVehicle(NULL), "Failed to parse vehicle: no element");
        TiXmlElement *elem = parser.getRoot()->FirstChildElement();
        IVehicle *temp = parser.parseVehicle(elem);
        EXPECT_TRUE(temp);
        elem = elem->NextSiblingElement();
        testing::internal::CaptureStderr();
        do {
            EXPECT_EQ(NULL, parser.parseVehicle(elem));
            elem = elem->NextSiblingElement();
        } while (elem != NULL);
        std::ofstream out("outputfiles/testoutputs/VehicleParserTester-ParseVehicle.txt");
        EXPECT_TRUE(out.is_open());
        out << testing::internal::GetCapturedStderr();
        out.close();
        EXPECT_TRUE(FileCompare("outputfiles/testoutputs/VehicleParserTester-ParseVehicle.txt",
                                "outputfiles/testoutputs/VehicleParserTester-ParseVehicle-expected.txt"));
    }
}

TEST_F(VehicleParserTester, GetVehicle) {
    VehicleParser parser;
    EXPECT_DEATH(parser.getVehicle(), "Failed to parse vehicle: no vehicle");
}

TEST_F(VehicleParserTester, ParseRoad) {
    VehicleParser parser;
    EXPECT_DEATH(parser.parseRoad(NULL), "Failed to parse road: no element");
}
