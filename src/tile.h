#ifndef _TILE_H_
#define _TILE_H_

#include <vector>
#include <utility>
using namespace std;

class tile
{
    public:
        tile(){}
        ~tile(){}
        vector<string> texts; //Text to display
        vector<pair<string, string> > links; //first = text second = textfile
        vector<pair<string, string> > vars; //first = varname second = varvalue
};

#endif
