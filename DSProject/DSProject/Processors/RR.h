#pragma once
#include "Processor.h"
#include"../Data Structures/Queue.h"

class RR : public Processor {
	int TSR;
	Queue<Process*> RDY_LIST;
public:
	RR(Scheduler* Sched,int tsr);
	virtual void ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Kill(int PID);
	virtual void Print();
	virtual int GET_QFT()const;
};