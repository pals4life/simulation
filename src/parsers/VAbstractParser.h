//============================================================================
// @name        : VAbstractParser.h
// @author      : Ward Gauderis
// @date        : 2/27/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Superclass for all parsers used in this project. Inspired by the XML-parser example code of Quinten Soetens.
//============================================================================

#ifndef SIMULATION_VABSTRACTPARSER_H
#define SIMULATION_VABSTRACTPARSER_H

#include "tinyxml/tinyxml.h"
#include <string>

class VAbstractParser {
public:
	bool loadFile(const std::string &kFilename);

	TiXmlElement *getRoot() const;

	void clear();

protected:
	const std::string readElement(TiXmlElement *element, const std::string &kTag);

private:
	TiXmlElement *fRoot;
	TiXmlDocument fDoc;
};


#endif //SIMULATION_VABSTRACTPARSER_H
