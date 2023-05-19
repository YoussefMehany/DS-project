#pragma once
#include "../Processors/Processor.h"
#include<iostream>
#include<string>
using namespace std;
class Output {
public:
	Output();
	void PrintInfo(Processor** Multiprocessors, int size, Queue<Process*> BLK, Queue<Process*> TRM, int timestep, InterfaceMode Mode);
	void PrintOut(string word, Colors color = WHITE);
	void PrintShow(string word, int duration, Colors color = WHITE);
	void LineBreaks(int count);
	void ClearConsole();
	void PrintColor(Colors clr);
	void PrintChar(char c, int count, Colors color = WHITE);
	void Intro();
	void ThankYou();
};