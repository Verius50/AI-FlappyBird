#include <iostream>
#pragma once
#define lenght 3
#define weight 2
class AIBird
{
	double weights[weight][lenght];

public:
	void InitRand();
	bool InputData(int* data);

	void print();
	void SaveWeights();
	void InitBestWeights(std::string path);

};

