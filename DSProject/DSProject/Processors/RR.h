#pragma once
#include "Processor.h"

class RR : public Processor {
	int TSR;
	Queue<Process*> RDY_LIST;
public:
	RR(Scheduler* Sched,int tsr);
	virtual void ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Lose(Process*& Stolen);
	virtual void Print();
};