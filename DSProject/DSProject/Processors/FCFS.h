#pragma once
#include "Processor.h"
#include "../Data Structures/LinkedQueue.h"

class FCFS: public Processor {
	LinkedQueue<Process> RDY_LIST;
public:
	FCFS();
	virtual Process* ScheduleAlgo();
	virtual void AddProcess(Process* process);
};