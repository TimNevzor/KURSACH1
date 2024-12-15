// calc.h
#pragma once
#include <vector>
#include <iostream>
class calc
{
public:
    float countoper(const std::vector<float>& vec){
    	float sumofsquares = 0.0f;
    	for (float value : vec) {
        	sumofsquares += value * value; 
    	}
    	//std::cout << sumofsquares << std::endl;
    	return sumofsquares;
    }
};

