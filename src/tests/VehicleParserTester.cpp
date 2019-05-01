//============================================================================
// @name        : VehicleParserTester.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for VehicleParser.
//============================================================================

#include <gtest/gtest.h>
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
        parser.parseVehicle(elem);
        elem = elem->NextSiblingElement();
        ASSERT_EXIT((parser.parseVehicle(elem), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
        elem = elem->NextSiblingElement();
        testing::internal::CaptureStderr();
        do {
            EXPECT_EQ(NULL, parser.parseVehicle(elem));
            elem = elem->NextSiblingElement();
        } while (elem != NULL);
        testing::internal::GetCapturedStderr();
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
