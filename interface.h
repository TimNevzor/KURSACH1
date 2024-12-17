//interface.h
#pragma once
#include <string>
#include <map>

class interface
    {
public:
        void get_args_of_comline(int argc, char* argv[]);
private:
        std::map<std::string, std::string> data_base;
    };
