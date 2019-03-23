//============================================================================
// @name        : VAbstractParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Superclass for all parsers used in this project. Inspired by the XML-parser example code of Quinten Soetens.
//============================================================================

#include <iostream>
#include "VAbstractParser.h"
#include "../tests/DesignByContract.h"

bool VAbstractParser::loadFile(const std::string &kFilename) {
	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling loadfile");
	REQUIRE(!kFilename.empty(), "Failed to load file: no filename");
	ENSURE(fDoc.LoadFile(kFilename.c_str()), fDoc.ErrorDesc());
	fRoot = fDoc.FirstChildElement();
	ENSURE(fRoot, "Failed to load file: no root element");
	return true;
}

TiXmlElement *VAbstractParser::getRoot() const {
	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling getRoot");
	ENSURE(fRoot, "Failed to get root: no root element");
	return fRoot;
}

const std::string VAbstractParser::readElement(TiXmlElement *const element, const std::string &kTag) {
	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling readElement");
    REQUIRE(!kTag.empty(), "Failed to read element: no tag");
    REQUIRE(element, "Failed to read element: no element");
	TiXmlElement *elem = element->FirstChildElement(kTag.c_str());
	if (elem) {
		TiXmlNode *node = elem->FirstChild();
		TiXmlText *text = node->ToText();
		return text->Value();
	}
	return "";
}

void VAbstractParser::clear() {
	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling clear");
	fDoc.Clear();
	fRoot = NULL;
}

bool VAbstractParser::properlyInitialized() const {
	return _initCheck == this;
}

VAbstractParser::VAbstractParser() {
	_initCheck = this;
	fRoot = NULL;
	ENSURE(this->properlyInitialized(), "Parser was not initialized when constructed");
}

VAbstractParser::~VAbstractParser() {
	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling destructor");
	clear();
}
