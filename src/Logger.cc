#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include "Logger.h"

string Logger::filename;
queue<string> Logger::qq;

void Logger::init()
{
	time_t rawTime;
	time(&rawTime);
	struct tm* timeInfo = localtime(&rawTime);
	string timeString = asctime(timeInfo);
	timeString.erase(timeString.end() - 1, timeString.end()); // Erase '\n'
	
	while (timeString.find_first_of(':', 0) != string::npos) // Remove invalid characters ':'
	{
		unsigned int position = timeString.find_first_of(':', 0);
		timeString.replace(position, 1, ".");
	}

	filename = "log " + timeString + ".txt";
}

void Logger::log(string s) //adds to queue
{
    qq.push(s);
}

void Logger::write() //writes to .log file
{
    ofstream logFile;
    logFile.open(filename, ios::app);

	if (logFile.is_open())
	{
		while(!qq.empty())
		{
			logFile << qq.front() + "\n";
			qq.pop();        
		}
		logFile.close();
	}
}