#include "parser.h"
#include <iostream>
#include <cctype>
#include <cstring>
#include <typeinfo>
#include <set>

Parser::Parser(){}
Parser::~Parser(){}

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

		//Button button(text, filename);
		Link l1;
		l1.filename = filename;
		l1.text = text;
		tileData.links.push_back(l1);
	}
}
bool Parser::isInt(string string)
{
	for (auto c : string)
	{
		if (!isdigit(c)) return false;
	}
	return true;
}
bool Parser::verify(const char* filename)
{
	rapidxml::file<> xmlFile(filename);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	
	bool verified = true;
	set<string> tileAllowed({"text", "link", "var", "if", "else"});
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
        cout << "\nFILE IS VERIFIED" << endl;
    else
        cout << "\nVERIFICATION ERROR" << endl;
    
	return verified;
	
}

//Given a node, verifyHelper() recursively analyzes the xml file
//Analyzes the given node, then recursively calls its child nodes
bool Parser::verifyHelper(rapidxml::xml_node<char>* node, set<string> reserved)
{
    bool isCorrect = true;
    
    cout << "\nAnalyzing tag: " << node->name() << endl;
    
    if(reserved.find(node->name()) != reserved.end())
    {
        cout << node->name() << " is a valid tag" << endl;
    }
    else
    {
        cerr<<"<"<<node->name()<<"> is an incorrect tag"
            <<"\nMust be one of: ";
        for(auto x = reserved.begin(); x != reserved.end(); x++)
        {
            cout <<"<"<<*x<<"> ";
        }
        cout << endl;
        return false;
    }
    
    if(strcmp(node->name(), "text") == 0)
    {
        reserved = set<string>();
        cout << "No more valid reserved words for parent \"text\""<<endl;
    }
    else if(strcmp(node->name(), "link") == 0)
    {
        reserved = set<string>({"file", "text"});
        cout << "New Reserved words: ";
        for(auto x = reserved.begin(); x != reserved.end(); x++)
        {
            cout << *x << ", ";
        }
        cout << endl;
    }
    else if(strcmp(node->name(), "var") == 0)
    {
        reserved = set<string>({"name", "value"});
        cout << "New Reserved words: ";
        for(auto x = reserved.begin(); x != reserved.end(); x++)
        {
            cout << *x << " ";
        }
        cout << endl;
    }
    else if(strcmp(node->name(), "if") == 0 || strcmp(node->name(), "else") == 0)
    {
        reserved = set<string>({"text", "link", "var"});
        cout << "New Reserved words: ";
        for(auto x = reserved.begin(); x != reserved.end(); x++)
        {
            cout << *x << " ";
        }
        cout << endl;
    }
    
    for(auto newNode = node->first_node();
        newNode;
        newNode = newNode->next_sibling())
    {
        cout << "\nRecursive call to node named \""<<newNode->name()
            <<"\" "<< "from node "<< node->name()<< endl;
        if(strcmp(newNode->name(), "") == 0)
        {
            cout << "Reached Leaf" << endl;
            return true;
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
						for (auto innerNode = node->first_node(); // Iterate through nodes within this conditional
							innerNode;
							innerNode = innerNode->next_sibling())
						{
							cout << "Found inner node: " << innerNode->name() << endl;
							grabXmlData(innerNode, newTile, boolVars, intVars, stringVars);
						}
					}
				}
				else if (intVars.find(varName) != intVars.end())
				{
					int expectedValue = atoi(checkValue);
					int actualValue = intVars.find(varName)->second;
					if (expectedValue == actualValue)
					{
						cout << varName << " test passed" << endl;
						for (auto innerNode = node->first_node(); 
							innerNode;
							innerNode = innerNode->next_sibling())
						{
							cout << "Found inner node: " << innerNode->name() << endl;
							grabXmlData(innerNode, newTile, boolVars, intVars, stringVars);
						}
					}
				}
				else if (stringVars.find(varName) != stringVars.end())
				{
					if (strcmp(checkValue, stringVars.find(varName)->second.c_str()) == 0)
					{
						cout << varName << " test passed" << endl;
						for (auto innerNode = node->first_node(); 
							innerNode;
							innerNode = innerNode->next_sibling())
						{
							cout << "Found inner node: " << innerNode->name() << endl;
							grabXmlData(innerNode, newTile, boolVars, intVars, stringVars);
						}
					}
				}
			}
			else
			{
				grabXmlData(node, newTile, boolVars, intVars, stringVars);
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
