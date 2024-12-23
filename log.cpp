//log.cpp
#include "log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

void logtxt::setpath(std::string path) 
{
	pathtologfile = path;
}

void logtxt::writeerr(const std::string& error)
{
	std::ofstream logfile(pathtologfile, std::ios::app); 
    auto now = std::time(nullptr);
    auto local_time = std::localtime(&now);
    std::ostringstream time_stream;
    time_stream << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    logfile << "[" << time_stream.str() << "] " << error << std::endl;
}
