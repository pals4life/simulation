//============================================================================
// @name        : VAbstractParser.cpp
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Superclass for all parsers used in this project. Inspired by the XML-parser example code of Quinten Soetens.
//============================================================================

#include "VAbstractParser.h"

bool VAbstractParser::loadFile(const std::string &filename) {
	if (!fdoc.LoadFile(filename)) {
		std::cerr << fdoc.ErrorDesc() << std::endl;
		return false;
	}
	froot = fdoc.FirstChildElement();
	if (froot == NULL) {
		std::cerr << "Failed to load file: No froot element." << std::endl; //TODO exception handling
		fdoc.Clear();
		return false;
	}
	return true;
}

TiXmlElement *VAbstractParser::getRoot() const {
	return froot;
}

const std::string VAbstractParser::readElement(TiXmlElement *const element, const std::string &tag) {
	TiXmlElement *elem = element->FirstChildElement(tag);
	TiXmlNode *node = elem->FirstChildElement();
	TiXmlText *text = node->ToText();
	return text->Value();
}
