//log.h
#pragma once
#include <string>

class logtxt
{
private:
    std::string pathtologfile;

public:
   /* logtxt()
    {
        pathtologfile = " ";
    };

    logtxt(const std::string& path)
    {
        pathtologfile = path;
    };*/
	void setpath(std::string path);
    void writeerr(const std::string& error);
};
