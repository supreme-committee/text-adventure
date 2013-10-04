#ifndef _TILE_H_
#define _TILE_H_

#include <vector>
#include <utility>
using namespace std;

typedef struct link
{
    string text;
    string filename;
}link; 

class tile
{
    public:
        tile(){}
        ~tile(){}
        vector<string> texts; //Text to display
        vector<link> links; //first = text second = textfile
};

#endif
