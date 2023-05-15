#pragma once
#include "../Processors/Processor.h"
#include<iostream>
#include<string>
#include "tabulate/table.hpp"
using namespace tabulate;
using namespace std;
using Row_t = Table::Row_t;
class Output {
public:
	Output();
	void PrintInfo(Processor** Multiprocessors, int size, Queue<Process*> BLK, Queue<Process*> TRM, int timestep);
	void PrintOut(string word);
	void ClearConsole();
	void PrintColor(Colors clr);
	void PrintReadme();
};