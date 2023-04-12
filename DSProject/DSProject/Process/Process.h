#pragma once
#include "../Enums.h"
#include "../Data Structures/LinkedQueue.h"
#include "../Data Structures/Pair.h"

class Processor;
class Process
{
private:
	Queue<Pair*> IO_LIST;
	static int counter;
	int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TerminationTime;
	int TurnAroundDuration;
	int WaitingTime;
	int IORequestTime;
	int IODuration;
	ProcessState State;
	Processor* RunProcessor;
public:
	Process();
	friend ostream& operator<<(ostream& out, Process& process);
	Process(int ArrivalTime, int PID, int CPUTime);
	int GetPID()const;
	int GetArrivalTime()const;
	int GetCPUTime()const;
	int GetTerminationTime()const;
	int GetResponseTime()const;
	int GetTurnAroundDuration()const;
	int GetWaitingTime()const;
	Pair& GetIO();
	void SetTerminationTime(int TerminationTime);
	void AddIO(int IO_R, int IO_D);
	void SetResponseTime(int FirstTime);
	void SetTurnAroundDuration();
	void SetWaitingTime();
	void SetState(ProcessState state);
	void SetProcessor(Processor* processor);
	Processor* GetProcessor();
	ProcessState GetState()const;
	
};

