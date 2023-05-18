#pragma once
#pragma once
#include "Processor.h"
#include "../Data Structures/Pri_Queue.h"

class EDF : public Processor {
	PriorityQueue<Process*> RDY_LIST;
public:
	EDF(Scheduler* Sched, int n);
	virtual void ScheduleAlgo();
	virtual void RunProcess();
	virtual void OverHeat();
	virtual void AddProcess(Process* process);
	virtual void Lose(Process*& Stolen);
	virtual void Print();

};
