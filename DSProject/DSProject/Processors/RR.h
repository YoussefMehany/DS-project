#pragma once
#include "Processor.h"
#include "../Data Structures/CircularLinkedList.h"

class RR : public Processor {
	CirLinkedList<Process> RDY_LIST;
public:
	RR();
	virtual Process* ScheduleAlgo();
	virtual void AddProcess(Process* process);
};