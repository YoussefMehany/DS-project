#pragma once
#include "Processor.h"
#include "../Data Structures/Pri_Queue.h"

class SJF : public Processor {
	PriorityQueue<Process*> RDY_LIST;
public:
	SJF(Scheduler* Sched,int n);
	virtual void ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void RunProcess();
	virtual void OverHeat();
	virtual void Lose(Process*& Stolen);
	virtual void Print();

};