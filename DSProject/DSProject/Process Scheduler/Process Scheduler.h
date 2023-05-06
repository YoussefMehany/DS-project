#pragma once
#include <cstdlib>
#include"../Process/Process.h"
#include"../Processors/Processor.h"
#include"../Data Structures/Queue.h"
#include"../Data Structures/Pri_Queue.h"
#include"../Data Structures/Pair.h"
#include"../UI Classes/Input.h"
#include"../UI Classes/Output.h"
#include <fstream>
#include <string>

class Scheduler
{
	ifstream InFile;
	ofstream OutFile;
	string Filename;
	Input* pIn;
	Output* pOut;
	int* IDs;
	int TimeStep, Turn;
	int NS, NF, NR, RTF, M, MaxW, STL, Fork_Prob;
	int Num_of_Processors; //Total number of processors
	int TTAT; //Total TurnAround time
	Queue<Process*>NEW, BLK, TRM;
	Queue<Pair<int, int>*> KILLSIG;
	Processor** MultiProcessor;
	Processor* LQ, * SQ;  //LQ->Longest Queue, SQ->Shortest Queue
	InterfaceMode Mode;
public:
	Scheduler();
	void Get_Data(); //Get Data from the input file
	void WriteData();
	void Set_Mode(InterfaceMode mode);
	void AddProcessors(int FCFCcnt, int SJFcnt, int RRcnt,int TSR);
	void TO_RDY(Process* P, int& i); //Move to RDY List
	void RET_TO_RDY(Process* P);
	void TO_TRM(Process* P); //Move to TRM List
	void TO_BLK(Process* P); //Move to BLK List
	void SchedulerUpdater(Processor* P);
	void UpdateInterface();	
	void WorkStealing();
	void FCFSMigration(Process* Migrate);
	void RRMigration(Process* Migrate);
	int  Get_TimeStep();
	int  Get_MaxW() const;
	int  Get_RTF() const;
	int  Get_NR()  const;
	int  Get_NS()  const;
	Processor* GetSQ() const;
	bool Simulation(); //Dividing the Processes on the processors
	Input* getInput();
	Output* getOutput();
	~Scheduler();
};