#pragma once
#include "Processor.h"
#include"../Data Structures/LinkedQueue.h"

class RR : public Processor {
	int TSR;
	Queue<Process*> RDY_LIST;
public:
	RR(Scheduler* Sched,int tsr);
	virtual Process* ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Print();
	virtual int GET_QFT()const;
};