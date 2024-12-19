//interface.cpp
#include <filesystem>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include "interface.h"
#include "log.h"
#include "error.h"
#include "server.h"

namespace po = boost::program_options;

std::tuple<std::string, std::string, std::string, std::string> interface::check_args(int argc, const char* argv[])
{
	std::string default_path_to_log = "log.txt";
	std::string default_path_to_base = "base.txt";
    std::string default_port = "33333";
	
    po::options_description desc("Допустимые опции");
    desc.add_options()
    
    ("help,h", "Вызов справки")
    ("log,l", po::value<std::string>()->default_value(default_path_to_log), "Путь до лог-файла")
    ("base,b", po::value<std::string>()->default_value(default_path_to_base), "Путь до базы данных")
    ("port,p", po::value<std::string>()->default_value(default_port), "Порт");

    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error &ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        return std::make_tuple("error", "", "", "");
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        std::cout << "Пример запуска сервера с ключами: " << std::endl;
        std::cout << "./main -l [путь до лог-файла] -b [путь до БД] -p [порт]" << std::endl;
        return std::make_tuple("help", "", "", "");
    }
    
	std::string path_to_log = vm["log"].as<std::string>();
    std::string path_to_base = vm["base"].as<std::string>();
    std::string port_str = vm["port"].as<std::string>();
    
    return std::make_tuple("ok", path_to_log, path_to_base, port_str);
} 

bool interface::check_path_to_logfile(std::string pathtologfile)
{
    try {
        if (!std::filesystem::exists(pathtologfile)) {
            throw criticalerr("Ошибка - Файл " + pathtologfile + " не существует");
        } else {
            std::cout << "Лог-файл по указанному пути существует" << std::endl;
        }
		return true;
    } catch (const criticalerr& e) {
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        return false;
    }
}

std::tuple<bool, std::map<std::string, std::string> > interface::check_path_to_database_and_get_database(std::string pathtodatabase, logtxt* logger)
{
    try {
        if (!std::filesystem::exists(pathtodatabase)) {
            throw criticalerr("Ошибка - Файл " + pathtodatabase + " не существует");
        } else {
            std::cout << "Файл базы данных по указанному пути существует" << std::endl;
        }

        std::ifstream file(pathtodatabase);
        std::map<std::string, std::string> logins;
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find(':');
            if (pos == std::string::npos) {
                throw noncriticalerr("Неверный формат строки: " + line);
            }
            std::string login = line.substr(0, pos);
            std::string password = line.substr(pos + 1);
            logins[login] = password;
        }

        data_base = logins;
        return std::make_tuple(true, data_base);

    } catch (const noncriticalerr& e) {
        std::cerr << "NONCRIT ERROR - " << e.what() << std::endl;
        logger->writeerr("NONCRIT ERROR - " + std::string(e.what()));
        return std::make_tuple(false, std::map<std::string, std::string>());
    }

    catch (const criticalerr& e) {
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        return std::make_tuple(false, std::map<std::string, std::string>());
    }
}

std::tuple<bool, int> interface::check_port(std::string portstr, logtxt* logger)
{
    try {
    	if (portstr == "") {
    		throw criticalerr("Некорректный пустой порт");
    	}
        int port;
        port = std::stoi(portstr);
        if (port < 1024 || port > 65535) {
            throw criticalerr("Некорректный порт");
        } else {
            std::cout << "Порт корректен" << std::endl;
            return std::make_tuple(true, port);
        }
    } catch (const criticalerr& e) {
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        return std::make_tuple(false, int());
    }
}

void interface::get_args_of_comline(int argc, const char* argv[])
{	
	std::string answer_of_check_args;
	std::string path_to_log;
	std::string path_to_base;
	std::string port_str;
	std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = check_args(argc, argv);
	
	if (answer_of_check_args == "help") {
		exit(0);
	}
	if (answer_of_check_args == "error") {
		exit(1);
	}
	
    logtxt logger;
    logger.setpath(path_to_log);

    bool fb;
    std::tie(fb, data_base) = check_path_to_database_and_get_database(path_to_base, &logger);

    bool fp;
    int port;
    std::tie(fp, port) = check_port(port_str, &logger);

   // std::cout << "Путь до лог-файла: " << path_to_log << std::endl;
  //  std::cout << "Путь до базы данных: " << path_to_base << std::endl;
  //  std::cout << "Порт: " << port << std::endl;
    server startconnect;
    startconnect.connection(port, data_base, &logger);
}
