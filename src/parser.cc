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
string Parser::trim(string input)
{
	while (input[0] == ' ')
	{
		input = input.replace(0, 1, ""); // Replace with empty string
	}
	int last = input.length() - 1;
	while (input[last] == ' ')
	{
		input = input.replace(last, 1, "");
		last--;
	}
	return input;
}

string Parser::insertVariable(string input, string thatString)
{
	auto start = input.find_first_of("{{");
	auto end = input.find_first_of("}}") + 2;
	string firstPart = input.substr(0, start);
	string secondPart = input.substr(end, string::npos);
	return firstPart + thatString + secondPart;
}

void Parser::grabXmlData(rapidxml::xml_node<char>* node, 
		Tile& tileData,
		map<string, bool>& boolVars,
		map<string, int>& intVars,
		map<string, string>& stringVars)
{
	if (strcmp(node->name(), "text") == 0) // Text tags
	{
		string nodeText(node->value());
		while (nodeText.find_first_of("{{") != string::npos) // Insert variable values if needed.
		{
			auto startSpot = nodeText.find_first_of("{{") + 2;
			auto endSpot = nodeText.find_first_of("}}");
			string varName = trim(nodeText.substr(startSpot, endSpot - startSpot));
			string varValue;
			if (boolVars.find(varName) != boolVars.end())
				boolVars.find(varName)->second == true ? varValue = "true" : varValue = "false";
			else if (intVars.find(varName) != intVars.end())
			{
				ostringstream ss;
				ss << intVars.find(varName)->second;
				varValue = ss.str();
			}
			else if (stringVars.find(varName) != stringVars.end())
				varValue = stringVars.find(varName)->second;
			else // Variable not found. Throw error and halt game
			{
				char errBuffer[256] = "Variable {{";
				strcat(errBuffer, varName.c_str());
				strcat(errBuffer, "}} could not be found!");
				throw rapidxml::parse_error(errBuffer, NULL);
			}
			nodeText = insertVariable(nodeText, varValue); // Replace value in string
		}
		tileData.texts.push_back(nodeText);
	}
	else if (strcmp(node->name(), "var") == 0)
	{
		string temp = trim(string(node->first_node()->value())); // Trim whitespace
		const char* varName = temp.c_str(); // Get var name
		char* varValue = node->last_node()->value(); // Get variable value

		if ((int)varName[0] > 122 || ( (int)varName[0] < 97 && (int)varName[0] > 90 ) ||         
            (int)varName[0] < 65) 
        { // Check that variable starts with letter
			char err[256] = "Invalid variable name: '";
			strcat(err, varName);
			strcat(err, "'. Variables must start with a letter (A-Z, a-z)");
			throw rapidxml::parse_error(err, NULL);
		}

		if (intVars.find(varName) != intVars.end()) // They're redefining an int variable
		{
			if (isInt(varValue)) { // New value is of the same type. Good!
				intVars.erase(intVars.find(varName));
				intVars.insert(pair<string, int>(varName, atoi(varValue)));
				Logger::log("WARNING: variable '" + string(varName) + "' already exists. Replacing its value...");
			}
			else { // Assigning non-int to int variable. Bad!
				char err[100] = "Attempt to assign non-int value to int variable: ";
				strcat(err, varName);
				throw rapidxml::parse_error(err, NULL);
			}
		}
		else if (boolVars.find(varName) != boolVars.end()) // They're redefining a bool variable
		{
			if (string(varValue) == "true" || string(varValue) == "false") {
				boolVars.erase(boolVars.find(varName));
				bool varAsBool = strcmp(varValue, "true") == 0 ? true : false;
				boolVars.insert(pair<string, bool>(varName, varAsBool));
				Logger::log("WARNING: variable '" + string(varName) + "' already exists. Replacing its value...");
			}
			else {
				char err[100] = "Attempt to assign non-bool value to bool variable: ";
				strcat(err, varName);
				throw rapidxml::parse_error(err, NULL);
			}
		}
		else if (stringVars.find(varName) != stringVars.end()) // They're redefining a string variable
		{
			stringVars.erase(stringVars.find(varName));
			Logger::log("WARNING: variable '" + string(varName) + "' already exists. Replacing its value...");
			stringVars.insert(pair<string, string>(varName, varValue));
		}
		else // Variable does not exist yet, so create it
		{
			if (isInt(varValue)) intVars.insert(pair<string, int>(varName, atoi(varValue)));
			else if (strcmp(varValue, "true") == 0 || strcmp(varValue, "false") == 0) {
				bool varAsBool = strcmp(varValue, "true") == 0 ? true : false;
				boolVars.insert(pair<string, bool>(varName, varAsBool));
			}
			else stringVars.insert(pair<string, string>(varName, varValue));
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

		if (strcmp(node->first_node()->name(), "var") == 0 &&
			strcmp(node->last_node()->name(), "amount") == 0)
		{
			variableName = node->first_node()->value();
			amount = node->last_node()->value();
		}
		else if (strcmp(node->first_node()->name(), "amount") == 0 &&
			strcmp(node->last_node()->name(), "var") == 0)
		{
			variableName = node->last_node()->value();
			amount = node->first_node()->value();
		}
		else
		{
			Logger::log("ERROR: tags within <modify></modify> are invalid");
			throw rapidxml::parse_error("tags within <modify></modify> are invalid", NULL);
			return;
		}

		// Check if int variable exists
		if (intVars.find(variableName) == intVars.end())
		{
			string message = "ERROR: attempt to modify an int that doesn't exist: " + string(variableName);
			Logger::log(message);
			return;
		}
		
		try
		{
			int i_amount = stoi(amount);
			auto woot = intVars.find(variableName);
			woot->second += i_amount;
		}
		catch (invalid_argument)
		{
			Logger::log("ERROR: invalid amount used in <modify> statement: " + string(amount));
			char err[256] = "invalid amount used in <modify> statement: ";
			strcat(err, amount);
			throw rapidxml::parse_error(err, NULL);
		}
	}
	else if (strcmp(node->name(), "image") == 0) // <image> tag
	{
		tileData.image = node->value();
	}
	else if (strcmp(node->name(), "sound") == 0) // <sound> tag
	{
		tileData.sfx = node->value();
	}
	else if (strcmp(node->name(), "music") == 0) // <music> tag
	{
		tileData.bgm = node->value();
	}
	else // Invalid tag
	{
		string er("ERROR: invalid tag found -> ");
		er.append(node->name());
		Logger::log(er);
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

bool Parser::compareInts(int one, int two, char* comparison)
{
	if (strcmp(comparison, "eq") == 0) { if (one == two) return true; }
	else if (strcmp(comparison, "gt") == 0) { if (one > two) return true; }
	else if (strcmp(comparison, "ge") == 0) { if (one >= two) return true; }
	else if (strcmp(comparison, "lt") == 0) { if (one < two) return true; }
	else if (strcmp(comparison, "le") == 0) { if (one <= two) return true; }
	else if (strcmp(comparison, "neq") == 0) { if (one != two) return true; }
	else
	{
		throw rapidxml::parse_error("Invalid comparison in <if> statement", NULL);
	}
	return false;
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
	string logString = "Starting verificaion of ";
	logString.append(filename);
	Logger::log(logString);

    fstream testFile;
    testFile.open(filename);
    if(!testFile.good())
    {
        Logger::log(string(filename) + " does not exist.");
        return false;

    }
    
	rapidxml::file<> xmlFile(filename);
	if (xmlFile.size() == 0) 
	{
		Logger::log(string(filename) + ": empty file!");
		return false;
	}
    
	rapidxml::xml_document<> doc;
	try
	{
		doc.parse<0>(xmlFile.data());
	}
	catch (rapidxml::parse_error e)
	{
		Logger::log(e.what());
		return false;
	}
	
	if (doc.first_node() == NULL)
	{
		Logger::log(string(filename) + ": invalid xml formatting");
		return false;
	}

	bool verified = true;
	//tiles allowed in a <tile> tag; to be passed to the recursive helper
#ifdef _MSC_VER // If Visual Studio is being used (it doesn't fully support init. lists yet).
	string tmp[] = {"text", "link", "var", "if", "else", "image", "modify"};
	set<string> tileAllowed(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
	set<string> tileAllowed({"text", "link", "var", "if", "else", "image", "modify"});
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
    else 
    {
        Logger::log(string("ERROR found in verification: ") + string(node->name()) +
            string(" is not a valid reserved word"));
        return false;
    }
    
    //Changing the reserved word list to be passed to next level of recursion
    if(strcmp(node->name(), "text") == 0 || strcmp(node->name(), "file") == 0
        || strcmp(node->name(), "name") == 0 || strcmp(node->name(), "value") == 0
            || strcmp(node->name(), "image") == 0)
    {
        reserved = set<string>();
        
        if(strcmp(node->name(), "file") == 0)
        {
            ifstream linkedFile;
            string filePath(".gamefiles/");
            filePath+=node->value();
            linkedFile.open(filePath);
            if(!linkedFile.is_open())
            {
                cout <<"NODE VALUE:"<< node->value() << endl;
                string s("File not found during verification: ");
                s.append(node->value());
                Logger::log(s);
                isCorrect = false;
            }
        }
    }
    else if(strcmp(node->name(), "link") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"file", "text"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"file", "text"}); //change reserved words
#endif
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
    else if(strcmp(node->name(), "else") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"text", "link", "var"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"text", "link", "var"});
#endif
        //printReserved(reserved);
    }
    else if(strcmp(node->name(), "if") == 0)
    {
        unsigned int count = 0;
        for(auto attr = node->first_attribute(); attr; attr=attr->next_attribute())
        {
            count++;
        }
        if(count != 3)
        {
            Logger::log("<if> tag does not have 3 attributes");
            return false;
        }
        
#ifdef _MSC_VER
		string tmp[] = {"text", "link", "var"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"text", "link", "var"});
#endif
        
        
    }
    else if(strcmp(node->name(), "modify") == 0)
    {
#ifdef _MSC_VER
		string tmp[] = {"var", "amount"};
		reserved = set<string>(tmp, tmp + sizeof(tmp)/sizeof(tmp[0]));
#else
        reserved = set<string>({"var", "amount"});
#endif
        printReserved(reserved);
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
    newTile.image = " ";
	try
	{
		rapidxml::file<> xmlFile(filename);

		if (xmlFile.size() < 6) { // File is too short to be valid
			Logger::log("ERROR: Invalid xml file: " + string(filename));
			throw rapidxml::parse_error("Invalid xml file", NULL);
			return Tile();
		}

		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		
		// Check if <tile> tag exists. Abort if it doesn't
		if (strcmp(doc.first_node()->name(), "tile") != 0)
		{
			Logger::log("ERROR: <tile> tag is missing!");
			throw rapidxml::parse_error("<tile> tag is missing", NULL);
			return Tile();
		}

		if (doc.first_node()->first_node() == NULL)
		{
			Logger::log("ERROR: no tags found within <tile> tags");
			throw rapidxml::parse_error("no tags found within <tile> tags", NULL);
			return Tile();
		}

		for (auto node = doc.first_node()->first_node();
			node;
			node = node->next_sibling())
		{
			bool ifPassed = false;
			if (strcmp(node->name(), "if") == 0) // Handle conditional statements
			{
				int count = 0;
				for (auto atr = node->first_attribute(); atr; atr = atr->next_attribute())
				{
					count++;
				}
				if (count > 3) throw rapidxml::parse_error("Too many arguments in if statement. Should be: arg1, arg2, and comparison", NULL);
				else if (count < 3) throw rapidxml::parse_error("if statement is missing arguments. Should be: arg1, arg2, and comparison", NULL);

				auto attr1 = node->first_attribute(); // arg1 (i.e. variable name)
				node->remove_first_attribute();
				auto attr2 = node->first_attribute(); // arg2
				node->remove_first_attribute();
				auto attr3 = node->first_attribute(); // Comparison (gt, ge, eq, etc.)
				node->remove_first_attribute();
				if (strcmp(attr1->name(), "arg1") != 0 ||
					strcmp(attr2->name(), "arg2") != 0 ||
					strcmp(attr3->name(), "comparison") != 0)
				{
					throw rapidxml::parse_error("Invalid if statement", NULL);
				}

				if (intVars.find(attr1->value()) != intVars.end()) // If arg1 is an int variable
				{
					if (isInt(attr2->value())) // arg2 is a plain int ("123")
					{ 
						ifPassed = compareInts(intVars.find(attr1->value())->second,
							atoi(attr2->value()),
							attr3->value());
					}
					else // arg2 two looks to be another stored int variable
					{ 
						if (intVars.find(attr2->value()) != intVars.end()) // Check that arg2 does indeed exist
						{
							ifPassed = compareInts(intVars.find(attr1->value())->second,
								intVars.find(attr2->value())->second,
								attr3->value());
						}
						else // arg2's variable doesn't exist. Error
						{
							char err[256] = "int variable referenced in arg2 not found: ";
							strcat(err, attr2->value());
							throw rapidxml::parse_error(err, NULL);
						}
					}
				}
				else if (boolVars.find(attr1->value()) != boolVars.end()) // If arg1 is a bool variable
				{
					if (strcmp(attr3->value(), "eq") != 0) // Invalid comparison used
						throw rapidxml::parse_error("Invalid comparison for bool variable. Bools can only be compared with \"eq\"", NULL);

					if (strcmp(attr2->value(), "true") == 0 || strcmp(attr2->value(), "false") == 0) // arg2 is "true" or "false"
					{
						bool comparisonValue = strcmp(attr2->value(), "true") == 0 ? true : false;
						ifPassed = boolVars.find(attr1->value())->second == comparisonValue;
					}
					else // arg2 is (presumably) a bool variable as well
					{ 
						if (boolVars.find(attr2->value()) != boolVars.end()) // Check that arg2 exists
						{
							ifPassed = boolVars.find(attr1->value())->second == boolVars.find(attr2->value())->second;
						}
						else // arg2 does not exist. Error
						{
							char err[256] = "bool variable referenced in arg2 not found: ";
							strcat(err, attr2->value());
							throw rapidxml::parse_error(err, NULL);
						}
					}
				}
				else if (stringVars.find(attr1->value()) != stringVars.end()) // If arg1 is a string variable
				{
					if (strcmp(attr3->value(), "eq") != 0) // Invalid comparison used
						throw rapidxml::parse_error("Invalid comparison for string variable. Strings can only be compared with \"eq\"", NULL);

					if (stringVars.find(attr2->value()) != stringVars.end()) // Check if arg2 is a string variable
					{
						ifPassed = string(attr1->value()) == stringVars.find(attr2->value())->second;
					}
					else // Just compare arg1 with arg2 as a plain string
					{
						ifPassed = string(stringVars.find(attr1->value())->second) == string(attr2->value());
					}
				}
				else // arg1 is not a variable. Throw an error
				{
					throw rapidxml::parse_error("arg1 in if statements must be a variable", NULL);
				}
			}
			else if (strcmp(node->name(), "else") != 0) // Handle all other tags (except <else>. It'll be handled seperately)
			{
				grabXmlData(node, newTile, boolVars, intVars, stringVars); // Grab data from this set of tags
			}

			if (ifPassed) // <if> passed. Grab all the stuff inside the <if></if> tags
			{
				grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
			}
			else if (ifPassed == false && // If the if statement failed, collect the stuff inside the <else></else> tags
				node->next_sibling() != NULL && 
				strcmp(node->next_sibling()->name(), "else") == 0)
			{
				node = node->next_sibling();
				grabInnerNodes(node, newTile, boolVars, intVars, stringVars);
			}
		}
	}
	catch (rapidxml::parse_error ex) // Format error as: "[[ERROR]] filename: ex.what()"
	{
		Logger::log(ex.what());
		string err = "[[ERROR]] ";
		err.append(filename).append(": ").append(ex.what());
		newTile.texts.clear();
		newTile.texts.push_back(err);
		return newTile;
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
#ifdef __APPLE__
    while(!saveFile.good())
    {
        cout << "File not found"<<endl;
        cout << "Enter full path to file: ";
        string s;
        cin >> s;
        saveFile.open(s);
    }
#endif 
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

