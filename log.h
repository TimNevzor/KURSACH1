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
    std::string pathtologfile; // путь к лог-файлу

public:
    // Конструктор для инициализации пути к лог-файлу
    logtxt()
    {
        pathtologfile = " ";
    };
    
    logtxt(const std::string& path)
    {
        pathtologfile = path;
    };
    
    int writeerr(const std::string& error)
    {
        std::ofstream logfile(pathtologfile, std::ios::app); // Открытие файла в режиме добавления
        if (!logfile) {
            throw criticalerr("Не удалось открыть лог-файл: " + pathtologfile);
        }

        auto now = std::time(nullptr);
        auto local_time = std::localtime(&now);
        // Форматирование времени
        std::ostringstream time_stream;
        time_stream << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
        // Запись ошибки в файл
        logfile << "[" << time_stream.str() << "] " << error << std::endl;

        return 0; // Возвращаем 0, если запись прошла успешно
    }
};
