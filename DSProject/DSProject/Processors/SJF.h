#pragma once
#include "Processor.h"
#include "../Data Structures/Pri_Queue.h"

class SJF : public Processor {
	PriorityQueue<Process*> RDY_LIST;
public:
	SJF(Scheduler* Sched);
	virtual void ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Kill(int PID);
	virtual void Print();
	virtual int GET_QFT()const;
};