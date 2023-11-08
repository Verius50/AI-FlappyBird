#include "AIBird.h"
#include <iostream>
#include <fstream>
#define weight 2
#define lenght 3

void AIBird::InitRand()
{
	for (int i = 0; i < weight; i++)
		for (int j = 0; j < lenght; j++)
			weights[i][j] = rand() % 100;
}

bool AIBird::InputData(int* data)
{
	double answ[2] = { 0.0 , 0.0 };
	for (int i = 0; i < weight; i++)
		for (int j = 0; j < lenght; j++)
			answ[i] += data[j] * weights[i][j];

	return answ[0] > answ[1] ? 0 : 1;
}

void AIBird::print()
{
	for (int i = 0; i < weight; i++)
		for (int j = 0; j < lenght; j++)
			std::cout << weights[i][j] << " ";
	std::cout << "\n";
}

void AIBird::SaveWeights()
{
	std::ofstream file("weights.txt");
	for (int i = 0; i < weight; i++)
		for (int j = 0; j < lenght; j++)
			file << weights[i][j] << ' ';
	file.close();
}

void AIBird::InitBestWeights(std::string path)
{
	int tmp = 0;
	std::ifstream file(path);
	for (int i = 0; i < weight; i++)
		for (int j = 0; j < lenght; j++)
		{
			file >> tmp;
			if(path == "weights.txt")
				weights[i][j] = tmp + rand() % 5 - rand() % 5;
			else weights[i][j] = tmp + rand() % 2 - rand() % 2;
		}
	file.close();

}


