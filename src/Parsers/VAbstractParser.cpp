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
	if (!doc.LoadFile(filename)) {
		std::cerr << doc.ErrorDesc() << std::endl;
		return false;
	}
	root = doc.FirstChildElement();
	if (root == NULL) {
		std::cerr << "Failed to load file: No root element." << std::endl;
		doc.Clear();
		return false;
	}
	return true;
}

TiXmlElement *VAbstractParser::getRoot() const {
	return root;
}

const std::string VAbstractParser::readElement(TiXmlElement *const element, const std::string &tag) {
	TiXmlElement *elem = element->FirstChildElement(tag);
	TiXmlNode *node = elem->FirstChildElement();
	TiXmlText *text = node->ToText();
	return text->Value();
}
