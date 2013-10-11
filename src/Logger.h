#include <queue>
#include <string>

using namespace std;

class Logger
{
    public:
        Logger(){};
        ~Logger(){};
        static void log(string); //adds to queue
        static void shutdown(); //clears queue
    private:
        static queue<string> qq;
        static void write(); //writes to .log file
};