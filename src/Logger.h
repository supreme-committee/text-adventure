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
		static void shutdown();
		static bool active();    // Returns true if qq still has stuff to empty
    private:
		static bool exitLoop;   // true if write() needs to kick out of infinite loop
		static string filename;
        static queue<string> qq;
};