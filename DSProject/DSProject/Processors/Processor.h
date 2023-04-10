#pragma once
#include "../Enums.h"
#include "../Process/Process.h"
#include"../Process Scheduler/Process Scheduler.h"
class Scheduler;
class Processor {

protected:
	int Total_CT;
	ProcessorState State;
public:
	Processor();
	//virtual functions
	virtual Process* ScheduleAlgo() =0;
	virtual void AddProcess(Process* process) =0;
	virtual void Print() = 0;
	virtual int GET_Total_CT()const = 0;
};