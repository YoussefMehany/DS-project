#pragma once
#include "Processor.h"
#include "../Data Structures/Pri_Queue.h"

class SJF : public Processor {
	PriorityQueue<Process> RDY_LIST;
public:
	SJF();
	virtual Process* ScheduleAlgo();
	virtual void AddProcess(Process* process);
};