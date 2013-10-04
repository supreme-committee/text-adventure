#include "parser.h"
#include <iostream>
#include <cctype>
#include <cstring>

Parser::Parser(){}
Parser::~Parser(){}
bool Parser::isInt(std::string string)
{
	for (auto c : string)
	{
		if (!isdigit(c)) return false;
	}
	return true;
}
bool Parser::verify(char* filename)
{
	return true;
}
tile Parser::parse(char* filename, 
				   std::map<std::string, bool>& boolVars,
				   std::map<std::string, int>& intVars,
				   std::map<std::string, std::string>& stringVars)
{
	tile newTile;
	try
	{
		rapidxml::file<> xmlFile(filename);
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		if (strcmp(doc.first_node()->name(), "tile") != 0)
		{
			std::cout << "tile group missing" << std::endl;
			return tile();
		}

		for (auto node = doc.first_node()->first_node();
			node;
			node = node->next_sibling())
		{
			std::cout << "Node found: " << node->name() << std::endl;
			if (strcmp(node->name(), "text") == 0)
			{
				newTile.texts.push_back(node->value());
			}
			else if (strcmp(node->name(), "var") == 0)
			{
				char* varName = node->first_node()->value();
				char* varValue = node->last_node()->value();
				if (isInt(varValue))
				{
					intVars.insert(std::pair<std::string, int>(varName, atoi(varValue)));
				}
				else if (strcmp(varValue, "true") == 0 || strcmp(varValue, "false") == 0)
				{
					strcmp(varValue, "true") == 0 ? 
						boolVars.insert(std::pair<std::string, bool>(varName, true)) :
						boolVars.insert(std::pair<std::string, bool>(varName, false));
				}
				else
				{
					stringVars.insert(std::pair<std::string, std::string>(varName, varValue));
				}
			}
			else if (strcmp(node->name(), "link") == 0)
			{
				char* filename = node->first_node()->value();
				char* text = node->last_node()->value();
			}
			else if (strcmp(node->name(), "if") == 0)
			{

			}
		}
	}
	catch (rapidxml::parse_error ex)
	{
		std::cout << ex.what() << std::endl;
		std::cout << ex.where<char>() << std::endl;
		return tile();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return tile();
	}

	std::cout << "Parse succeeded!" << std::endl;
	return newTile;
}
