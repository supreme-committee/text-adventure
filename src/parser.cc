#include "parser.h"
#include "Logger.h"
#include <iostream>
#include <cctype>
#include <cstring>
#include <typeinfo>
#include <set>
#include <fstream>
#include <sstream>

Parser::Parser(){}
Parser::~Parser(){}

// ======================== PRIVATE FUNCTIONS ========================

void Parser::grabXmlData(rapidxml::xml_node<char>* node, 
		Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars)
{
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
			if (intVars.find(varName) != intVars.end()) // If variable already exists, replace its value
			{
				intVars.erase(intVars.find(varName));
				string logString(varName);
				Logger::log("WARNING: variable '" + logString + "' already exists. Replacing its value...");
			}
			intVars.insert(pair<string, int>(varName, atoi(varValue)));
		}
		else if (strcmp(varValue, "true") == 0 || strcmp(varValue, "false") == 0)
		{
			bool varValueBool = strcmp(varValue, "true") == 0 ? true : false;
			if (boolVars.find(varName) != boolVars.end()) // If variable already exists, replace its value
			{
				boolVars.erase(boolVars.find(varName));
				string logString(varName);
				Logger::log("WARNING: variable '" + logString + "' already exists. Replacing its value...");
			}
			boolVars.insert(pair<string, bool>(varName, varValueBool));
		}
		else
		{
			if (stringVars.find(varName) != stringVars.end())
			{
				stringVars.erase(stringVars.find(varName));
				string logString(varName);
				Logger::log("WARNING: variable '" + logString + "' already exists. Replacing its value...");
			}
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
	else if (strcmp(node->name(), "modify") == 0)
	{
		char* variableName = NULL;
		char* amount;
		int i_amount;

		if (strcmp(node->first_node()->name(), "var") == 0)
		{
			variableName = node->first_node()->value();
			amount = node->last_node()->value();
		}
		else if (strcmp(node->first_node()->name(), "amount") == 0)
		{
			variableName = node->last_node()->value();
			amount = node->first_node()->value();
		}
		else
		{
			Logger::log("ERROR: tags within <modify></modify> are out of order");
			return;
		}

		// Check if int variable exists
		if (intVars.find(variableName) == intVars.end())
		{
			string message = "ERROR: attempt to modify an int that doesn't exist: ";
			message.append(variableName);
			Logger::log(message);
			return;
		}

		stringstream ss(amount); // Modify the int
		ss >> i_amount;
		auto woot = intVars.find(variableName);
		woot->second += i_amount;
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
	string logString = "Starting verication of ";
	logString.append(filename);
	Logger::log(logString);

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
		Logger::log("ERROR: Tile tag missing");
	    verified = false;
	}
    
    if(verified)
	{
		char logString[200] = "Passed verification: ";
		Logger::log(strcat(logString, filename));
	}
    else
	{
		char logString[200] = "ERROR: file failed verification: ";
		Logger::log(strcat(logString, filename));
	}

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
    
    if(reserved.find(node->name()) != reserved.end()) //checks if tag is a reserved word
    {
	
    }
    else //prints out all valid tags passed into this function
    {
        return false;
    }
    
    //Changing the reserved word list to be passed to next level of recursion
    if(strcmp(node->name(), "text") == 0 || strcmp(node->name(), "file") == 0
        || strcmp(node->name(), "name") == 0 || strcmp(node->name(), "value") == 0)
    {
        reserved = set<string>();
    }
    else if(strcmp(node->name(), "link") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"file", "text"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"file", "text"}); //change reserved words
#endif
        
        ifstream linkedFile;
        linkedFile.open(node->value());
        if(!linkedFile.good())
        {
            string s("File not found during verification: ");
            s+=node->value();
            Logger::log(s);
        }
        else
            cout << "File found: " << node->value();
        //printReserved(reserved);
    }
    else if(strcmp(node->name(), "var") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"name", "value"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"name", "value"});
#endif
        //printReserved(reserved);
    }
    else if(strcmp(node->name(), "if") == 0 || strcmp(node->name(), "else") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"text", "link", "var"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"text", "link", "var"});
#endif
        //printReserved(reserved);
    }
    
    for(auto newNode = node->first_node();
        newNode;
        newNode = newNode->next_sibling())
    {
        if(strcmp(newNode->name(), "") == 0)
        {
            if(newNode->type() == rapidxml::node_data)
            {
                continue;
            }
            else
            {
                return true;
            }
        }

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
	char logString[200] = "Started parse on: ";
	Logger::log(strcat(logString, filename));
	Tile newTile;
	try
	{
		rapidxml::file<> xmlFile(filename);
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		// Check if <tile> tag exists. Abort if it doesn't
		if (strcmp(doc.first_node()->name(), "tile") != 0)
		{
			Logger::log("ERROR: <tile> tag is missing!");
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

				if (boolVars.find(varName) != boolVars.end())
				{
					bool expectedValue = strcmp(checkValue, "true") == 0 ? true : false;
					bool actualValue = boolVars.find(varName)->second;
					if (expectedValue == actualValue)
					{
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
						grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
					}
					else ifPassed = false;
				}
				else if (stringVars.find(varName) != stringVars.end())
				{
					if (strcmp(checkValue, stringVars.find(varName)->second.c_str()) == 0)
					{
						grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
					}
					else ifPassed = false;
				}
				else // Variable does not exist
				{
					// The condition passes if checking a bool for false
					if (strcmp(checkValue, "false") == 0) grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
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
				grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
			}
		}
	}
	catch (rapidxml::parse_error ex)
	{
		Logger::log(ex.what());
		Logger::log(ex.where<char>());
		return Tile();
	}
	catch (exception e)
	{
		Logger::log(e.what());
		return Tile();
	}

	char parseSucceed[200] = "Parsing succeeded: ";
	Logger::log(strcat(parseSucceed, filename));
	return newTile;
}

void Parser::save(string saveFileName, string tarName, string tileFile,
	                map<string, bool>& boolVars,
	                map<string, int>& intVars,
	                map<string, string>& stringVars)
{
    ofstream saveFile;
    saveFile.open(saveFileName);
    saveFile << tarName << endl;
    saveFile << tileFile << endl;
    
    saveFile << "bools" << endl;
    for(map<string, bool>::iterator i = boolVars.begin();
        i != boolVars.end(); i++)
    {
        saveFile << (*i).first << " " << (*i).second << endl;
    }
    
    saveFile << "ints" << endl;
    for(map<string, int>::iterator i = intVars.begin();
        i != intVars.end(); i++)
    {
        saveFile << (*i).first << " " << (*i).second << endl;
    }
    
    saveFile << "strings" << endl;
    for(map<string, string>::iterator i = stringVars.begin();
        i != stringVars.end(); i++)
    {
        saveFile << (*i).first << " " << (*i).second << endl;
    }
    saveFile.close();
}

void Parser::load(string openFileName, string& tarName, string& tileFile,
                    map<string, bool>& boolVars,
                    map<string, int>& intVars,
                    map<string, string>& stringVars)
{
    //clear the var tables
    boolVars.clear();
    intVars.clear();
    stringVars.clear();
    
    //Open the file and put tar and tile info in right places.
    ifstream saveFile;
    saveFile.open(openFileName);
    getline(saveFile,tarName);
    getline(saveFile,tileFile);
    
    //Check to see if file is in the right format by checking the variable header
    string typeCheck("bools");
    string checkee;
    getline(saveFile,checkee);
    if(typeCheck.compare(checkee) != 0)
    {
        cerr << "Loading failed: Save file corrupted" << endl;
    }
    
    //Load in bools to the boolVar table
    getline(saveFile,checkee);
    while(checkee.compare("ints") != 0)
    {
        stringstream ss(checkee);
        string first;
        bool second;
        ss >> first;
        ss >> second;
        pair<string,bool> tomap(first, second);
        boolVars.insert(tomap);
        getline(saveFile,checkee);
    }
    
    //Load in ints. checkee should now == "ints"
    getline(saveFile,checkee);
    while(checkee.compare("strings") != 0)
    {
        stringstream ss(checkee);
        string first;
        int second;
        ss >> first;
        ss >> second;
        pair<string,int> tomap(first, second);
        intVars.insert(tomap);
        getline(saveFile,checkee);
    }
    
    //Load in strings. checkee == "strings" 
    //Scan until EOF
    while(saveFile.good())
    {
        getline(saveFile,checkee);
        stringstream ss(checkee);
        string first;
        string second;
        ss >> first;
        ss >> second;
        if(first.empty())
            break;
        pair<string,string> tomap(first, second);
        stringVars.insert(tomap);
    }
    
    saveFile.close();
}

