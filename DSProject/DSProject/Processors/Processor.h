#pragma once
#include "../Enums.h"
#include "../Process/Process.h"
class Processor {

protected:
	ProcessorState State;
public:
	Processor();
	virtual Process* ScheduleAlgo() =0;
	virtual void AddProcess(Process* process) =0;
};