#pragma once
#include "Processor.h"
#include"../Data Structures/LinkedQueue.h"

class RR : public Processor {
	Queue<Process*> RDY_LIST;
public:
	RR();
	virtual Process* ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Print();
	virtual int GET_Total_CT()const;
};