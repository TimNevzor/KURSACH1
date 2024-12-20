/** @file
 * @author Невзоров Т.В.
 * @version 1.0
 * @date 20.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля interface
 */
#pragma once
#include "log.h"
#include "error.h"
#include <filesystem>
#include <string>
#include <map>
#include <tuple>
/** @brief Прием и обработка аргументов командной строки
 * @details В private секцию устанавливается ассоциативный массив "логин : пароль"
 * Устанавливает порт и пути до лог-файла и базы данных.
 * Порождает объекты других классов и передает им параметры
 * Для обработки ключей и их параметров используются отдельные функции
 */
class interface
    {
public:
        /**
        * @brief Первичный прием аргументов командной строки
        * @details Порождает все последующие методы и объекты других классов, а именно
        * объект logger класса logtxt, объект startconnect класса server 
        * @param argc Количество аргументов командной строки
        * @param argv Массив строк, содержащий аргументы командной строки
        */
        void get_args_of_comline(int argc, const char* argv[]);
        /**
        * @brief Проверка аргументов командной строки
        * @details Проверяет введенные пользователем флаги и их параметры
        * @param argc Количество аргументов командной строки
        * @param argv Массив строк, содержащий аргументы командной строки
        * @throw criticalerr при вводе несуществующего ключа или ключа без параметра
        */
        std::tuple<std::string, std::string, std::string, std::string> check_args(int argc, const char* argv[]);
        /**
        * @brief Проверка пути до лог-файла
        * @details Проверяет существование лог-файла по указанному пути, устанавливает путь до лог-файла
        * @param pathtologfile Путь до лог-файла
        * @throw criticalerr при вводе несуществующего пути до лог-файла
        */
        bool check_path_to_logfile(std::string pathtologfile);
        /**
        * @brief Проверка пути до базы данных и формирование ассоциативного массива
        * @details Проверяет существование пути до базы данных и формирует ассоциативный массив из логинов и паролей, 
        * устанавливает путь до базы данных
        * @param pathtodatabase Путь до базы данных
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл
        * @throw criticalerr при вводе несуществующего пути до базы данных
        * @throw noncriticalerr при неверном формате строки
        */
        std::tuple<bool, std::map<std::string, std::string> > check_path_to_database_and_get_database(std::string pathtodatabase, logtxt* logger);
        /**
        * @brief Проверка порта
        * @details Проверяет введенный пользователем порт на корректность и приводит его к типу int,
        * устанавливает порт
        * @param portstr Строка, содержащая порт
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл
        * @throw criticalerr при вводе пустого или некорректного порта
        */
        std::tuple<bool, int> check_port(std::string portstr, logtxt* logger);
private:
        std::map<std::string, std::string> data_base; ///< База данных
    };
