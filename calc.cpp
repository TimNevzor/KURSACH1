// calc.cpp
#include "calc.h"

float calc::process_vector(const std::vector<float>& vec)
{
    float sum = 0.0f;
    for (float value : vec) {
        sum += value * value; // Возведение в квадрат и суммирование
    }
    return sum;
}
