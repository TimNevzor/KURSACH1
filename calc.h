/** @file
 * @author Невзоров Т.В.
 * @version 1.0
 * @date 20.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля calc
 */
#pragma once
#include <vector>
/** @brief Прием и обработка вектора чисел типа float
 * @details Производимая над числами операция - сумма квадратов
 */
class calc
{
public:
    /**
    * @brief Метод, производящий математические вычисления
    * @details Принимает вектор чисел типа float и производит математическую операцию "сумма квадратов".
    * Возвращает сумму квадратов полученных чисел
    * @param vec Ссылка на вектор чисел типа float
    */
    float countoper(const std::vector<float>& vec);
};
