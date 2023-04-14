#pragma once
#include "../Processors/Processor.h"
#include<iostream>
#include<string>

using namespace std;

class Output {
public:
	Output();
	void PrintInfo(Processor** Multiprocessors, int size, Queue<Process*> BLK, Queue<Process*> TRM, int timestep);
	void PrintOut(string word);
};