#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include "Logger.h"

queue<string> Logger::qq;

void Logger::log(string s) //adds to queue
{
    qq.push(s);
}

void Logger::shutdown() //clears queue
{
    write();
}

void Logger::write() //writes to .log file
{
    ofstream logFile;
    
	time_t rawTime;
	time(&rawTime);
	struct tm* timeInfo = localtime(&rawTime);
	string timeString = asctime(timeInfo);
	timeString.erase(timeString.end() - 1, timeString.end()); // Erase '\n'
	
	while (timeString.find_first_of(':', 0) != string::npos) // Remove invalid character ':'
	{
		unsigned int position = timeString.find_first_of(':', 0);
		timeString.replace(position, 1, ".");
	}

	string file = "log " + timeString + ".txt";

    logFile.open(file, ios::app);
	if (logFile.is_open())
	{
		while(!qq.empty())
		{
			logFile << qq.front();
			qq.pop();        
		}
		logFile.close();
	}
}