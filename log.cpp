//log.cpp
#include "log.h"
#include "error.h"
#include "interface.h"
#include <system_error>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

int logtxt::writeerr(const std::string& error)
{
    std::ofstream logfile(pathtologfile, std::ios::app); // Открытие файла в режиме добавления
    if (!logfile) {
        throw criticalerr("CRIT ERROR - Не удалось открыть лог-файл: " + pathtologfile);
    }

    auto now = std::time(nullptr);
    auto local_time = std::localtime(&now);
    std::ostringstream time_stream;
    time_stream << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    logfile << "[" << time_stream.str() << "] " << error << std::endl;

    return 0;
}
