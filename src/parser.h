#ifndef _PARSER_H_
#define _PARSER_H_
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "tile.h"
#include <map>
using namespace std;

class Parser
{
private:
	// This will grab all data except for conditionals
	static void grabXmlData(rapidxml::xml_node<char>* node, 
		Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars);

public:
	Parser();
	~Parser();
	static bool isInt(string string);
	static bool verify(char* filename);
	static Tile parse(const char* filename, 
				   map<string, bool>& boolVars,
				   map<string, int>& intVars,
				   map<string, string>& stringVars);
};

#endif
