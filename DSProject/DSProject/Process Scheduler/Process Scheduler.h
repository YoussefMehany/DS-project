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
	int TimeStep;
	int NS, NF, NR, ND, RTF, M, INIT_M, MaxW, STL, Fork_Prob, Heat_Prob, n;
	int ProcessesRTF, ProcessesMaxW, ProcessesStolen, ProcessesKilled;
	int Num_of_Processors; //Total number of processors
	int TTAT; //Total TurnAround time
	Queue<Process*>NEW, BLK, TRM;
	Queue<Pair<int, int>*> KILLSIG;
	Processor** MultiProcessor;
	Processor* LQ, * SQ, * SFCFS, * SSJF, * SRR ,*SEDF;  //LQ->Longest Queue, SQ->Shortest Queue //Rest are shortest Queues for each specific Processor
	InterfaceMode Mode;
public:
	Scheduler();
	void Get_Data(); //Get Data from the input file
	void WriteData();
	void Set_Mode(InterfaceMode mode);
	void AddProcessors(int FCFCcnt, int SJFcnt, int RRcnt, int EDFcnt, int TSR, int n);
	void TO_SHORTEST_RDY(Process* P, bool fcfs = false);
	void TO_TRM(Process* P); //Move to TRM List
	void TO_BLK(Process* P); //Move to BLK List
	void UpdateInterface();
	void WorkStealing();
	void CoolingSystem(bool fcfs = false);
	void FCFSMigration(Process* Migrate);
	void RRMigration(Process* Migrate);
	void DecideLongest();
	void DecideShortest(int Type);
	Process* AddChildToSQ(int RemCPU, int DeadLine);
	int Get_TimeStep();
	int Get_MaxW() const;
	int Get_FP() const;
	int Get_RTF() const;
	int Get_NR()  const;
	int Get_NS()  const;
	Processor* GetSQ() const;
	Processor* GetSSJF() const;
	Processor* GetSRR() const;
	Processor* GetSFCFS() const;
	bool Simulation(); //Dividing the Processes on the processors
	bool MakeIO(Process* Block);
	Input* getInput();
	Output* getOutput();
	~Scheduler();
};