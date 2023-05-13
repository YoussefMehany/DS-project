#pragma once
#include "../Enums.h"
#include"../Data Structures/Queue.h"
#include "../Data Structures/Pair.h"


class Processor;
class Process
{
private:
	Queue<Pair<int, int>*> IO_LIST; //Queue or Pri ? not specified in the document
	static int counter, TTAT, LastID;
	int PID;
	int TIOD; // Total IO Duration
	int ArrivalTime;
	int ResponseTime;
	int CPUTime,CPUTemp;
	int TerminationTime;
	int TurnAroundDuration;
	int WaitingTime;
	int LastRunTime;
	ProcessState State;
	Processor* RunProcessor;
	Process* Lchild, *Rchild, *Parent;
public:
	Process();
	friend ostream& operator<<(ostream& out,const Process& process);
	Process(int ArrivalTime, int CPUTime, int PID = -1);
	int GetPID()const;
	int GetArrivalTime()const;
	int GetCPUTime()const;
	int GetTIOD()const;
	int GetTerminationTime()const;
	int GetResponseTime()const;
	int GetTurnAroundDuration()const;
	int GetWaitingTime()const;
	int GetCurrWaitingTime(int TimeStep)const;
	int GetCPURemainingTime()const;
	Pair<int, int>* GetIO();
	Process* GetLeftChild()const;
	Process* GetRightChild()const;
	Process* GetParent()const;
	Processor* GetProcessor()const;
	ProcessState GetState()const;
	void AddWaitingTime(int Time);
	void SetTerminationTime(int TerminationTime);
	void SetResponseTime(int FirstTime);
	void SetTurnAroundDuration();
	void SetTIOD(int TIOD);
	void SetWaitingTime();
	void SetState(ProcessState state);
	void SetProcessor(Processor* processor);
	void SetLeftChild(Process* child);
	void SetRightChild(Process* child);
	void SetParent(Process* parent);
	void AddIO(int IO_R, int IO_D);	
	void NextIO();
	void UpdateInfo();
	void SetChildrenState(bool par = true);
	void PrintOutFile(ofstream& out);
	static int GetTTAT();
};

