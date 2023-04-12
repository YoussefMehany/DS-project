#pragma once
#include "../Enums.h"
#include "../Process/Process.h"

class Scheduler;
class Processor {

protected:
	int Total_CT;
	static int Counter;
	int ID;
	Process* R;
	ProcessorState State;
	Scheduler* S;
public:
	Processor(Scheduler* Sched);
	virtual Process* ScheduleAlgo() =0;
	virtual void AddProcess(Process* process) =0;
	virtual void Print() = 0;
	virtual int GET_Total_CT()const = 0;
	void UpdateState(Process* process);
};