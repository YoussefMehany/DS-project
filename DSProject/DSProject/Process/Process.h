#pragma once
#include "Enums.h"
class Process
{
private:
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
public:
	Process(int ArrivalTime, int CPUTime , int IORequestTime , int IODuration);
	int GetPID()const;
	int GetArrivalTime()const;
	int GetCPUTime()const;
	int GetTerminationTime()const;
	int GetResponseTime()const;
	int GetTurnAroundDuration()const;
	int GetWaitingTime()const;
	int GetIORequestTime()const;
	int GetIODuration()const;
	void SetTerminationTime(int TerminationTime);
	void SetResponseTime(int FirstTime);
	void SetTurnAroundDuration();
	void SetWaitingTime();
};

