//============================================================================
// @name        : RoadParserTester.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for RoadParser.
//============================================================================

#include <gtest/gtest.h>
#include "../parsers/RoadParser.h"

class RoadParserTester : public ::testing::Test {
protected:
    friend class RoadParser;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(RoadParserTester, Init) {
    RoadParser parser;
    EXPECT_DEATH(parser.getRoad(), "Failed to parse road: no road");
}

TEST_F(RoadParserTester, ParseRoad) {
    RoadParser parser;
    testing::internal::CaptureStderr();
    bool loaded = parser.loadFile("inputfiles/testinputs/test2.xml");
    EXPECT_EQ(true, loaded);
    testing::internal::GetCapturedStderr();
    if (loaded) {
        EXPECT_DEATH(parser.parseRoad(NULL), "Failed to parse road: no element");
        TiXmlElement *elem = parser.getRoot()->FirstChildElement();
        parser.parseRoad(elem);
        elem = elem->NextSiblingElement();
        testing::internal::CaptureStderr();
        do {
            EXPECT_EQ(NULL, parser.parseRoad(elem));
            elem = elem->NextSiblingElement();
        } while (elem != NULL);
        testing::internal::GetCapturedStderr();
    }
}

TEST_F(RoadParserTester, GetRoad) {
    RoadParser parser;
    EXPECT_DEATH(parser.getRoad(), "Failed to parse road: no road");
}

TEST_F(RoadParserTester, ParseConnection) {
    RoadParser parser;
    EXPECT_DEATH(parser.parseConnection(NULL), "Failed to parse connection: no element");
}
