#ifndef _PARSER_H_
#define _PARSER_H_
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "tile.h"
#include <map>

class Parser
{
public:
	Parser();
	~Parser();
	static bool isInt(std::string string);
	static bool verify(char* filename);
	static tile parse(char* filename, 
				   std::map<std::string, bool>& boolVars,
				   std::map<std::string, int>& intVars,
				   std::map<std::string, std::string>& stringVars);
};

#endif
