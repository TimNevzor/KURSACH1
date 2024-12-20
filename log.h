/** @file
 * @author Невзоров Т.В.
 * @version 1.0
 * @date 20.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля log
 */
#pragma once
#include <string>
/** @brief Прием и запись ошибок в лог-файл
 * @details В private секцию устанавливается путь до лог-файла
 */
class logtxt
{
private:
    std::string pathtologfile; ///< Путь до лог-файла
public:
    /**
        * @brief Установка пути до лог-файла
        * @details Устанавливает путь до лог-файла
        * @param path Строка, представляющая собой путь до лог-файла
        */
    void setpath(std::string path);
    /**
        * @brief Запись ошибки в лог-файл
        * @details Вычисляет дату и время происхождения ошибки,
        * форматирует их и записывает вместе с ошбкой в лог-файл
        * @param error Константная ссылка на строку, представляющая собой ошибку
        */
    void writeerr(const std::string& error);
};
