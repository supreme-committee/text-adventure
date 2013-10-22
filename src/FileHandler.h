#ifndef _FILEHANDLER_H_
#define _FILEHANDLER_H_
#include <string>
using namespace std;

#ifdef WIN32
#include <Windows.h>
#elif __APPLE__ // ???
#endif

class FileHandler
{
public:
	enum class OpenFileMode {SAVEGAME, NEWGAME};
	static string openFile(OpenFileMode mode);
	static string saveFile();
};

#endif