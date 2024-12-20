/** @file
 * @author Невзоров Т.В.
 * @version 1.0
 * @date 20.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля error
 */
#pragma once
#include <stdexcept>
#include <string>
/**
 * @brief Класс для обработки критических ошибок
 * @details Этот класс наследуется от std::runtime_error и используется для
 * обработки ошибок и исключений.
 */
class criticalerr : public std::runtime_error
{
public:
    /**
    * @brief Конструктор, принимающий константную ссылку на строку.
    * @param s Константная ссылка на строку, сообщение об ошибке в виде строки.
    */
    criticalerr(const std::string& s) : std::runtime_error(s) {}
};
/**
 * @brief Класс для обработки некритических ошибок
 * @details Этот класс наследуется от std::runtime_error и используется для
 * обработки ошибок и исключений.
 */
class noncriticalerr : public std::runtime_error
{
public:
    /**
    * @brief Конструктор, принимающий константную ссылку на строку.
    * @param s Константная ссылка на строку, сообщение об ошибке в виде строки.
    */
    noncriticalerr(const std::string& s) : std::runtime_error(s) {}
};
