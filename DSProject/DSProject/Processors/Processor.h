#pragma once
#include "../Enums.h"
#include "../Process/Process.h"

class Scheduler;
class Processor {

protected:
	static int Counter;
	int QFT,ID,TBT,TIT; //TBT->Total Busy Time ,TIT->Total Idle Time, QFT->Queue Finish Time
	double pLoad, pUtil;
	Process* R;
	ProcessorState State;
	Scheduler* S;
public:
	Processor(Scheduler* Sched);
	virtual void ScheduleAlgo() =0;
	virtual void AddProcess(Process* process) =0;
	virtual void Print() = 0;
	virtual int GET_QFT()const = 0;
	void UpdateState();
	ProcessorState Get_State()const;
	Process* Get_Run()const;
};