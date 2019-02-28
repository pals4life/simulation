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

bool VAbstractParser::loadFile(const std::string &kFilename) {
	if (!fDoc.LoadFile(kFilename.c_str())) {
		std::cerr << fDoc.ErrorDesc() << std::endl;
		return false;
	}
	fRoot = fDoc.FirstChildElement();
	if (fRoot == NULL) {
		std::cerr << "Failed to load file: No root element." << std::endl; //TODO exception handling
		fDoc.Clear();
		return false;
	}
	return true;
}

TiXmlElement *VAbstractParser::getRoot() const {
	return fRoot;
}

const std::string VAbstractParser::readElement(TiXmlElement *const element, const std::string &kTag) {
	TiXmlElement *elem = element->FirstChildElement(kTag.c_str());
	TiXmlNode *node = elem->FirstChild();
	TiXmlText *text = node->ToText();
	return text->Value();
}
