#pragma once
#include <cstdlib>
#include"../Process/Process.h"
#include"../Processors/Processor.h"
#include"../Data Structures/CircularLinkedList.h"
#include"../Data Structures/LinkedQueue.h"
#include"../Data Structures/Pri_Queue.h"
#include"../Data Structures/Pair.h"
#include"../UI Classes/Input.h"
#include"../UI Classes/Output.h"
#include <fstream>
#include <string>

class Scheduler
{
	ifstream InFile;
	string Filename;
	Input* pIn;
	Output* pOut;
	int TimeStep, Turn;
	int NS, NF, NR, TSR, RTF, M, MaxW, STL, Fork_Prob;
	int Num_of_Processors; //Total number of processors
	Queue<Process*>NEW, BLK, TRM, RUN;
	Queue<Pair*> KILLSIG;
	Processor** MultiProcessor;
public:
	Scheduler();
	void Get_Data(); //Get Data from the input file
	void AddProcessors(int FCFCcnt, int SJFcnt, int RRcnt);
	//Processor* Find_Shortest(int l, int r); //Find Shortest Queue
	bool Processing(); //Dividing the Processes on the processors
	void TO_RDY(Process* P, int& i); //Move to RDY List
	void RET_TO_RDY(Process* P);
	void TO_RUN(Process* P); //Move to RUN List
	void TO_TRM(Process* P); //Move to TRM List
	void TO_BLK(Process* P); //Move to BLK List
	void SchedulerUpdater(Process* P);
	void PrintData()const;
	Input* getInput() {
		return pIn;
	}
	Output* getOutput() {
		return pOut;
	}
};