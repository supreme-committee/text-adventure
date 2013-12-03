#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <thread>
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

	while (timeString.find_first_of(' ', 0) != string::npos) // Remove spaces
	{
		unsigned int position = timeString.find_first_of(' ', 0);
		timeString.replace(position, 1, "_");
	}

	filename = "log." + timeString + ".txt";
}

void Logger::log(string s) //adds to queue
{
	if (filename.length() > 0) // If logger is initialized
	{
		qq.push(s);
		write();
	}
}

void Logger::write() //writes to .log file
{
	if (!qq.empty()) // If queue is not empty
	{
		ofstream logFile;
		logFile.open(filename, ios::app);

		if (logFile.is_open()) // If file successfully opened
		{
			while(!qq.empty()) // Dequeue data until queue is empty
			{
				logFile << "[" << getTime() << "] " << qq.front() + "\n";
				qq.pop();        
			}
			logFile.close();
		}
	}
}

string Logger::getTime()
{
	time_t currentTime;
	time(&currentTime);
	struct tm* timeInfo = localtime(&currentTime);
	
	ostringstream timeStream;
	timeStream << timeInfo->tm_hour;
	timeStream << ":";
	timeStream << timeInfo->tm_min;
	timeStream << ":";
	timeStream << timeInfo->tm_sec;

	return timeStream.str();
}