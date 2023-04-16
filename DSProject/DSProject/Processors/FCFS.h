#pragma once
#include "Processor.h"
#include"../Data Structures/List.h"

class FCFS: public Processor {
	List<Process*> RDY_LIST;
public:
	FCFS(Scheduler* Sched);
	virtual void ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Kill(int PID);
	virtual void Print();
	virtual int GET_QFT()const;
};