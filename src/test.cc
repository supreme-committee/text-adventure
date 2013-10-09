#include <iostream>
#include <string>
#include <map>
#include "parser.h"

using namespace std;

int main()
{
    map<string, bool> boolVars;
	map<string, int> intVars;
	map<string, string> stringVars;
    Parser::verify("testing/Start.xml");
}