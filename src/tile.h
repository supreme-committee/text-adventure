#ifndef _TILE_H_
#define _TILE_H_

#include <vector>
#include <utility>
using namespace std;

typedef struct Link
{
    string text;
    string filename;
}Link; 

class Tile
{
    public:
        Tile(){}
        ~Tile(){}
        vector<string> texts; //Text to display
        vector<Link> links; //first = text second = textfile
};

#endif
