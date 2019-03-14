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

	/**
	 * 	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling loadfile");
	 *	REQUIRE(!kFilename.empty(), "Failed to load file: no filename");
	 *	ENSURE(fDoc.LoadFile(kFilename.c_str()), fDoc.ErrorDesc());
	 *	ENSURE(fRoot, "Failed to load file: no root element");
	 */
	bool loadFile(const std::string &kFilename);

	/**
	 * 	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling getRoot");
	 *	ENSURE(fRoot, "Failed to get root: no root element");
	 */
	TiXmlElement *getRoot() const;

	/**
	 * 	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling clear");
	 */
	void clear();

	bool properlyInitialized() const;

	/**
	 * 	ENSURE(this->properlyInitialized(), "Parser was not initialized when constructed");
	 */
	VAbstractParser();

	/**
	 * 	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling destructor");
	 */
	virtual ~VAbstractParser();

protected:

	/**
	 * 	REQUIRE(this->properlyInitialized(), "Parser was not initialized when calling readElement");
	 *	REQUIRE(element, "Failed to read element: no element");
	 *	REQUIRE(!kTag.empty(), "Failed to read element: no tag");
	 */
	const std::string readElement(TiXmlElement *element, const std::string &kTag);

private:
	TiXmlElement *fRoot;
	TiXmlDocument fDoc;
	VAbstractParser *_initCheck;
};


#endif //SIMULATION_VABSTRACTPARSER_H
