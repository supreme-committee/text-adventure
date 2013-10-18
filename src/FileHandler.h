#ifndef _FILEHANDLER_H_
#define _FILEHANDLER_H_
#include <string>
using namespace std;

#ifdef WIN32
#include <Windows.h>
#elif APPLE // ???
#endif

class FileHandler
{
public:
	static string openFile();
	static void saveFile();
};

#endif