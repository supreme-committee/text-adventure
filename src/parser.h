#ifndef _PARSER_H_
#define _PARSER_H_
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "tile.h"
#include <map>
#include <set>
using namespace std;

class Parser
{
private:
	// This will extract data (variables, text, etc.) from the given node. 
	// It'll not handle conditionals
	static void grabXmlData(rapidxml::xml_node<char>* node, 
		Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars);

	// Iterates through child nodes of the given node and calls grabXmlData on each one
	static void grabInnerNodes(rapidxml::xml_node<char>* node, Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars);

	static void printReserved(set<string> reserved);

public:
	Parser();
	~Parser();
	static bool isInt(string string);
	static bool verify(const char* filename);
	static bool verifyHelper(rapidxml::xml_node<char>* node, set<string> reserved);
	static Tile parse(const char* filename, 
				   map<string, bool>& boolVars,
				   map<string, int>& intVars,
				   map<string, string>& stringVars);
	static void save(string saveFileName, string tarName, string tileFile,
    	                map<string, bool>& boolVars,
    	                map<string, int>& intVars,
    	                map<string, string>& stringVars);
    static void load(string openFileName, string& tarName, string& tileFile,
                        map<string, bool>& boolVars,
                        map<string, int>& intVars,
                        map<string, string>& stringVars);
};

#endif
