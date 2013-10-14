#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <thread>
#include "Logger.h"

bool Logger::exitLoop;
string Logger::filename;
queue<string> Logger::qq;

void Logger::init()
{
	exitLoop = false;

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
	while (true) // Loop indefinitely until queue is emptied
	{
		if (!qq.empty())
		{
			ofstream logFile;
			logFile.open(filename, ios::app);

			if (logFile.is_open())
			{
				while(!qq.empty())
				{
					logFile << "[" << getTime() << "] " << qq.front() + "\n";
					qq.pop();        
				}
				logFile.close();
			}
		}
		else 
		{
			if (exitLoop) break; // qq is empty and logger thread needs to close. Exit this loop
			this_thread::sleep_for(chrono::milliseconds(1000)); // Sleep for 1 second
		}
	}
}

void Logger::shutdown()
{
	exitLoop = true;
}

bool Logger::active()
{
	return qq.size() != 0;
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