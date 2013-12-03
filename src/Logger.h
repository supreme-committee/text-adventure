#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <queue>
#include <string>

using namespace std;

class Logger
{
    public:
        Logger(){};
        ~Logger(){};
		static void init();      // Initialize the logger
        static void log(string); //adds to queue
		static void write();     //writes to .log file

    private:
		static string filename;
        static queue<string> qq;
		static string getTime(); // Returns the current time "[h:m:s]" (e.g. [7:34:23])
};

#endif