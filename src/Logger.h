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
};