//calc.cpp
#include "calc.h"
#include <vector>
#include <iostream>

float calc::countoper(const std::vector<float>& vec)
{
    float sumofsquares = 0.0f;
    for (float value : vec) {
        sumofsquares += value * value;
    }
    return sumofsquares;
}
