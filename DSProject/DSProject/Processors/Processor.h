#pragma once
#include "../Enums.h"
#include "../Process/Process.h"

class Scheduler;
class Processor {

protected:
	static int Counter, OverHeated;
	int QFT, ID, TBT, TIT,N_TEMP, N; //TBT->Total Busy Time ,TIT->Total Idle Time, QFT->Queue Finish Time, N timesteps for overheating
	double pLoad, pUtil;
	Process* R;
	ProcessorState State;
	Scheduler* S;
public:
	Processor(Scheduler* Sched , int n);
	virtual void ScheduleAlgo() = 0;
	virtual void AddProcess(Process* process) = 0;
	virtual void Print() = 0;
	virtual void Lose(Process*& Stolen) = 0;
	void AddToRun();
	virtual void RunProcess() = 0;
	virtual void OverHeat() = 0;
	double Get_pUtil() const;
	double Get_pLoad() const;
	int GET_QFT()const;
	int Get_ID()const;
	int Get_HeatFactor()const;
	void Reset_HeatFactor();
	void UpdateState();
	void SetProcessorState(ProcessorState processor_state);
	ProcessorState Get_State()const;
	Process* Get_Run()const;
};