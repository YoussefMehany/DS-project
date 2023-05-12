#pragma once
#include "Processor.h"

class RR : public Processor {
	int TSR,TSRTemp;
	Queue<Process*> RDY_LIST;
	void Round();
public:
	RR(Scheduler* Sched,int tsr,int n);
	void RRMigration();
	virtual void ScheduleAlgo();
	virtual void AddProcess(Process* process);
	virtual void Lose(Process*& Stolen);
	virtual void Print();
};