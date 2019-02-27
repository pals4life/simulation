//============================================================================
// @name        : NetworkParser.cpp
// @author      : Ward Gauderis
// @date        : 2/26/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Parse a Network from XML.
//============================================================================

#include "NetworkParser.h"
#include <tinyxml.h>

Network *NetworkParser::parseNetwork(TiXmlElement *const element) {
	for (TiXmlElement *elem = element->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

	}
	return network;
}

Network *NetworkParser::getNetwork() {
	return network;
}

NetworkParser::NetworkParser() {
	network = new Network;
}

NetworkParser::~NetworkParser() {

}
