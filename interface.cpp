#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "interface.h"
#include "server.h"

namespace po = boost::program_options;



void interface::get_args_of_comline(int argc, char* argv[])
{
    std::string default_path_to_base = "/home/stud/Desktop/KURSACH/base.txt";
    std::string default_path_to_log = "/home/stud/Desktop/KURSACH/log.txt";
    std::string default_port = "33333";

    po::options_description desc("Допустимые опции");
    desc.add_options()
    ("help,h", "Вызов справки")
    ("base,b", po::value<std::string>()->default_value(default_path_to_base), "Путь до базы данных")
    ("log,l", po::value<std::string>()->default_value(default_path_to_log), "Путь до лог-файла")
    ("port,p", po::value<std::string>()->default_value(default_port), "Порт");

    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error &ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        exit(1);
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }

    std::string path_to_base = vm["base"].as<std::string>();
    std::string path_to_log = vm["log"].as<std::string>();
    std::string port_str = vm["port"].as<std::string>();

    logtxt sansara(path_to_log);

    try {
        std::ifstream file_of_base(path_to_base);
        if (!file_of_base) {
            throw criticalerr("Ошибка: Файл базы данных не найден по указанному пути: " + path_to_base);
        }

        int port = 0;
        port = std::stoi(port_str);
        if (port < 1024 || port > 65535) {
            throw criticalerr("НЕКОРРЕКТНЫЙ ПОРТ. КРИТИЧЕСКАЯ ОШИБКА");
        }

        std::cout << "Путь до базы данных: " << path_to_base << std::endl;
        std::cout << "Путь до лог-файла: " << path_to_log << std::endl;
        std::cout << "Порт: " << port << std::endl;

        

        try {
            std::ifstream file(path_to_base);
            if (!file.is_open()) {
                throw criticalerr("CRIT ERR - Не открывается файл БД");
            }

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
            std::cout << "Логины и пароли: " << std::endl;

            for (const auto& pair : logins) {
                std::cout << pair.first << " : " << pair.second << std::endl;
            }

            //return data_base;
        } catch (const criticalerr& e) {
            std::cerr << e.what() << std::endl;
            sansara.writeerr(e.what());
            exit(1);
        } catch (const noncriticalerr& e) {
            std::cerr << e.what() << std::endl;
            sansara.writeerr(e.what());
            
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
            sansara.writeerr(e.what());
            exit(1);
        }



       server startconnect;
      startconnect.connection(port, data_base, &sansara);
        
        
    } catch (const criticalerr& e) {
        std::cerr << e.what() << std::endl;
        sansara.writeerr(e.what());
        exit(1);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        sansara.writeerr(e.what());
        exit(1);
    }
}
