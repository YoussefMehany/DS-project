#pragma once
#include <cstdlib>
#include"../Process/Process.h"
#include"../Processors/Processor.h"
#include"../Data Structures/CircularLinkedList.h"
#include"../Data Structures/LinkedQueue.h"
#include"../Data Structures/Pri_Queue.h"
class Scheduler
{
	int TimeStep;
	int NS, NF, NR, RTF, M, MaxW, STL, Fork_Prob;
	int Num_of_processors; //Total number of processors
	Queue<Process*>NEW, BLK, TRM;
	Processor** MultiProcessor;
public:
	Scheduler();
	void Get_Data(); //Get Data from the input file
	Processor* Find_Shortest(int l, int r); //Find Shortest Queue
	bool Processing(); //Dividing the Processes on the processors
	void To_RDY(Process* P); //Move to RDY List
	void TO_TRM(Process* P); //Move to TRM List
	void TO_BLK(Process* P); //Move to BLK List
	void PrintData()const;
};