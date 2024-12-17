//log.h
#pragma once
#include <system_error>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "error.h"
#include "interface.h"

class logtxt
{
private:
    std::string pathtologfile;

public:
    logtxt()
    {
        pathtologfile = " ";
    };

    logtxt(const std::string& path)
    {
        pathtologfile = path;
    };

    int writeerr(const std::string& error);
};
