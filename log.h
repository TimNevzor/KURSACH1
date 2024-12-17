//log.h
#pragma once
#include <string>

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
