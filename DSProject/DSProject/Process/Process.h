#pragma once
#include "../Enums.h"
#include"../Data Structures/Queue.h"
#include "../Data Structures/Pair.h"

class Processor;
class Process
{
private:
	Queue<Pair<int>*> IO_LIST;
	static int counter;
	int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TerminationTime;
	int TurnAroundDuration;
	int WaitingTime;
	ProcessState State;
	Processor* RunProcessor;
	Process* Child;
public:
	Process();
	friend ostream& operator<<(ostream& out,const Process& process);
	Process(int ArrivalTime, int PID, int CPUTime);
	int GetPID()const;
	int GetArrivalTime()const;
	int GetCPUTime()const;
	int GetTerminationTime()const;
	int GetResponseTime()const;
	int GetTurnAroundDuration()const;
	int GetWaitingTime()const;
	Pair<int>& GetIO();
	Process* GetChild()const;
	Processor* GetProcessor()const;
	ProcessState GetState()const;
	void SetTerminationTime(int TerminationTime);
	void SetResponseTime(int FirstTime);
	void SetTurnAroundDuration();
	void SetWaitingTime();
	void SetState(ProcessState state);
	void SetProcessor(Processor* processor);
	void SetChild(Process* child);
	void AddIO(int IO_R, int IO_D);
	
};

