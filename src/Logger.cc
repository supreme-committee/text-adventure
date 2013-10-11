#include <iostream>
#include <fstream>
#include <time.h>
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
    
    long int i = time(NULL);
    string file;
    stringstream strstream;
    strstream << i;
    strstream >> file;
    file += ".txt";
    logFile.open(file, ios::app);
    while(!qq.empty())
    {
        logFile << qq.front();
        qq.pop();        
    }
    logFile.close();
}