//interface.h
#pragma once
#include "log.h"
#include "error.h"
#include <filesystem>
#include <string>
#include <map>
#include <tuple>

class interface
    {
public:
        void get_args_of_comline(int argc, const char* argv[]);
        std::tuple<std::string, std::string, std::string, std::string> check_args(int argc, const char* argv[]); 
        bool check_path_to_logfile(std::string pathtologfile);
        std::tuple<bool, std::map<std::string, std::string> > check_path_to_database_and_get_database(std::string pathtodatabase, logtxt* logger);
        std::tuple<bool, int> check_port(std::string portstr, logtxt* logger);
        //std::map<std::string, std::string> get_database(std::string pathtodatabase, logtxt* logger);
private:
        std::map<std::string, std::string> data_base;
    };
