#include "parser.h"
#include <iostream>
#include <cctype>
#include <cstring>
#include <typeinfo>
#include <set>

Parser::Parser(){}
Parser::~Parser(){}

// ======================== PRIVATE FUNCTIONS ========================

void Parser::grabXmlData(rapidxml::xml_node<char>* node, 
		Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars)
{
	cout << "Node found: " << node->name() << endl;
	if (strcmp(node->name(), "text") == 0)
	{
		tileData.texts.push_back(node->value());
	}
	else if (strcmp(node->name(), "var") == 0)
	{
		char* varName = node->first_node()->value();
		char* varValue = node->last_node()->value();
		if (isInt(varValue))
		{
			intVars.insert(pair<string, int>(varName, atoi(varValue)));
		}
		else if (strcmp(varValue, "true") == 0 || strcmp(varValue, "false") == 0)
		{
			strcmp(varValue, "true") == 0 ? 
				boolVars.insert(pair<string, bool>(varName, true)) :
				boolVars.insert(pair<string, bool>(varName, false));
		}
		else
		{
			stringVars.insert(pair<string, string>(varName, varValue));
		}
	}
	else if (strcmp(node->name(), "link") == 0)
	{
		char* filename;
		char* text;

		if (strcmp(node->first_node()->name(), "file") == 0)
		{
			filename = node->first_node()->value();
			text = node->last_node()->value();
		}
		else
		{
			filename = node->last_node()->value();
			text = node->first_node()->value();
		}

		// Create a link and add it to the Tile object
		Link l1;
		l1.filename = filename;
		l1.text = text;
		tileData.links.push_back(l1);
	}
}

void Parser::grabInnerNodes(rapidxml::xml_node<char>* node, Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars)
{
	for (auto innerNode = node->first_node(); // Iterate through nodes within this conditional
		innerNode;
		innerNode = innerNode->next_sibling())
	{
		grabXmlData(innerNode, tileData, boolVars, intVars, stringVars);
	}
}

// =================== PUBLIC FUNCTIONS =====================

bool Parser::isInt(string string)
{
	for (auto c : string)
	{
		if (!isdigit(c)) return false;
	}
	return true;
}

//Returns true if the given xml file conforms to the story file standard
//Will print out to cerr all debug/test text.
bool Parser::verify(const char* filename)
{
    cout << endl;
	rapidxml::file<> xmlFile(filename);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	
	bool verified = true;
	//tiles allowed in a <tile> tag; to be passed to the recursive helper
#ifdef _MSC_VER // If Visual Studio is being used (it doesn't fully support init. lists yet).
	string tmp[] = {"text", "link", "var", "if", "else"};
	set<string> tileAllowed(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
	set<string> tileAllowed({"text", "link", "var", "if", "else"});
#endif
	if(strcmp(doc.first_node()->name(), "tile") == 0)
	{
        for(auto newNode = doc.first_node()->first_node();
            newNode;
            newNode = newNode->next_sibling())
        {
            if(!verifyHelper(newNode, tileAllowed))
            {
                verified = false;
            }
        }
    }
	else
	{
	    cerr << "Tile Tag Missing!" << endl;
	    verified = false;
	}
    
    if(verified)
        cerr << "\nFILE IS VERIFIED" << endl;
    else
        cerr << "\nVERIFICATION ERROR" << endl;
    
	return verified;
	
}

//Helper function to cut down on copy pasting or printing new reserved words 
void Parser::printReserved(set<string> reserved)
{
    cerr << "New Reserved words: ";  
    for(auto x = reserved.begin(); x != reserved.end(); x++)
    {
        cerr << *x << ", ";
    }
    cerr << endl;
}

//Given a node, verifyHelper() recursively analyzes the xml file
//Analyzes the given node, then recursively calls its child nodes
bool Parser::verifyHelper(rapidxml::xml_node<char>* node, set<string> reserved)
{
    bool isCorrect = true;
    
    cerr << "Analyzing tag: " << node->name() << endl;
    
    if(reserved.find(node->name()) != reserved.end()) //checks if tag is a reserved word
    {
        cerr << node->name() << " is a valid tag" << endl;
    }
    else //prints out all valid tags passed into this function
    {
        cerr<<"<"<<node->name()<<"> is an incorrect tag"
            <<"\nMust be one of: ";
        for(auto x = reserved.begin(); x != reserved.end(); x++)
        {
            cerr <<"<"<<*x<<"> ";
        }
        cerr << endl;
        return false;
    }
    
    //Changing the reserved word list to be passed to next level of recursion
    if(strcmp(node->name(), "text") == 0 || strcmp(node->name(), "file") == 0
        || strcmp(node->name(), "name") == 0 || strcmp(node->name(), "value") == 0)
    {
        reserved = set<string>();
        cerr << "No valid reserved words for \""<<node->name()<<
            "\""<<endl;
    }
    else if(strcmp(node->name(), "link") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"file", "text"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"file", "text"}); //change reserved words
#endif
        printReserved(reserved);
    }
    else if(strcmp(node->name(), "var") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"name", "value"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"name", "value"});
#endif
        printReserved(reserved);
    }
    else if(strcmp(node->name(), "if") == 0 || strcmp(node->name(), "else") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"text", "link", "var"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"text", "link", "var"});
#endif
        printReserved(reserved);
    }
    
    for(auto newNode = node->first_node();
        newNode;
        newNode = newNode->next_sibling())
    {
        cerr << "\nRecursive call to node named \""<<newNode->name()
            <<"\" "<< "from node "<< node->name()<< endl;
        if(strcmp(newNode->name(), "") == 0)
        {
            if(newNode->type() == rapidxml::node_data)
            {
                cerr << "Reached XML value data belonging to: "<<node->name()<<endl;
                continue;
            }
            else
            {
                cerr << "Reached Leaf" << endl<<endl;
                return true;
            }
        }
        //cout << newNode << endl;
        if(!verifyHelper(newNode, reserved))
        {
            isCorrect = false;
        }
    }
    return isCorrect;
}

Tile Parser::parse(const char* filename, 
				   map<string, bool>& boolVars,
				   map<string, int>& intVars,
				   map<string, string>& stringVars)
{
	Tile newTile;
	try
	{
		rapidxml::file<> xmlFile(filename);
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		if (strcmp(doc.first_node()->name(), "tile") != 0)
		{
			cout << "tile group missing" << endl;
			return Tile();
		}

		for (auto node = doc.first_node()->first_node();
			node;
			node = node->next_sibling())
		{
			bool ifPassed = true;
			if (strcmp(node->name(), "if") == 0) // Handle conditional statements
			{
				auto attr = node->first_attribute();
				char* varName = attr->name();
				char* checkValue = attr->value();

				cout << "check if " << attr->name() << " equals " << attr->value() << endl;
					
				if (boolVars.find(varName) != boolVars.end())
				{
					bool expectedValue = strcmp(checkValue, "true") == 0 ? true : false;
					bool actualValue = boolVars.find(varName)->second;
					if (expectedValue == actualValue)
					{
						cout << varName << " test passed" << endl;
						grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
					}
					else ifPassed = false;
				}
				else if (intVars.find(varName) != intVars.end())
				{
					int expectedValue = atoi(checkValue);
					int actualValue = intVars.find(varName)->second;
					if (expectedValue == actualValue)
					{
						cout << varName << " test passed" << endl;
						grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
					}
					else ifPassed = false;
				}
				else if (stringVars.find(varName) != stringVars.end())
				{
					if (strcmp(checkValue, stringVars.find(varName)->second.c_str()) == 0)
					{
						cout << varName << " test passed" << endl;
						grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
					}
					else ifPassed = false;
				}
			}
			else if (strcmp(node->name(), "else") != 0) // Handle all other tags
			{
				grabXmlData(node, newTile, boolVars, intVars, stringVars);
			}

			// If the if statement failed, collect the stuff inside the <else></else> tags
			if (ifPassed == false && 
				node->next_sibling() != NULL && 
				strcmp(node->next_sibling()->name(), "else") == 0)
			{
				node = node->next_sibling();
				cout << "If failed. Using stuff in <else>" << endl;
				grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
			}
		}
	}
	catch (rapidxml::parse_error ex)
	{
		cout << ex.what() << endl;
		cout << ex.where<char>() << endl;
		return Tile();
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return Tile();
	}

	cout << "Parse succeeded!" << endl;
	return newTile;
}
