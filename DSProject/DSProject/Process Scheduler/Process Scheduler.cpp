#include"Process Scheduler.h"
#include "../Processors/Processor.h"
#include "../Processors/FCFS.h"
#include "../Processors/RR.h"
#include "../Processors/SJF.h"
#include <windows.h>
Scheduler::Scheduler()
{
	TimeStep = NS = NF = NR = Num_of_Processors = RTF = M = MaxW = STL = Fork_Prob = Turn = 0;
	MultiProcessor = nullptr;
	pIn = new Input;
	srand(time(0));
	Mode = Interactive;
}
void Scheduler::Set_Mode(InterfaceMode mode) { Mode = mode; }
void Scheduler::AddProcessors(int FCFScnt, int SJFcnt, int RRcnt, int TSR)
{
	Num_of_Processors = FCFScnt + SJFcnt + RRcnt;
	MultiProcessor = new Processor * [Num_of_Processors];
	int cnt = 0;
	while (FCFScnt--) {
		MultiProcessor[cnt++] = new FCFS(this);
	}
	while (SJFcnt--) {
		MultiProcessor[cnt++] = new SJF(this);
	}
	while (RRcnt--) {
		MultiProcessor[cnt++] = new RR(this, TSR);
	}
}
void Scheduler::Get_Data()
{
	int TSR = 0;
	pIn->GetFileName(Filename);
	InFile.open(Filename + ".txt");
	InFile >> NF >> NS >> NR >> TSR >> RTF >> MaxW >> STL >> Fork_Prob >> M;
	for (int i = 0; i < M; i++) {
		int AT, ID, CPU, N, IO_R, IO_D;
		InFile >> AT >> ID >> CPU >> N;
		Process* process = new Process(AT, ID, CPU);
		while (N-- > 0) {
			char dummy;
			InFile >> dummy >> IO_R >> dummy >> IO_D >> dummy;
			if (N > 0) InFile >> dummy;
			process->AddIO(IO_R, IO_D);
		}
		NEW.enqueue(process);
	}
	AddProcessors(NF, NS, NR, TSR);
	int T, PID;
	while (InFile >> T) {
		InFile >> PID;
		Pair<int>* kill = new Pair<int>(T, PID);
		KILLSIG.enqueue(kill);
	}
}
//Processor* Scheduler::Find_Shortest( int l, int r) // for phase2
//{
//	Processor* Min = MultiProcessor[0];
//	for (int i = l; i <= r; i++)
//	{
//		if (MultiProcessor[i]->GET_Total_CT() < Min->GET_Total_CT())
//			Min = MultiProcessor[i];
//	}
//	return Min;
//}
bool Scheduler::Processing()
{
	TimeStep++;
	if (TRM.GetSize() == M)
		return false;

	while (!NEW.isEmpty()) {
		Process* temp = nullptr;
		NEW.peek(temp);
		if (TimeStep == temp->GetArrivalTime()) {
			NEW.dequeue(temp);
			TO_RDY(temp, Turn);
		}
		else break;
	}
	int Rand = 0;

	for (int i = 0; i < Num_of_Processors; i++) //Move process from RDY to RUN if processor is IDLE and Make the probability checking
	{
		Process* process = nullptr;
		process = MultiProcessor[i]->ScheduleAlgo();
	}
	for (int i = 0; i < Num_of_Processors; i++) {
		if (MultiProcessor[i]->Get_State() == BUSY) {
			Rand = 1 + rand() % 100;
			Process* process = MultiProcessor[i]->Get_Run();
			if (Rand <= 15) {
				TO_BLK(process);
				SchedulerUpdater(MultiProcessor[i]);
			}
			else if (Rand >= 20 && Rand <= 30) {
				RET_TO_RDY(process);
			}
			else if (Rand >= 50 && Rand <= 60) {
				TO_TRM(process);
				SchedulerUpdater(MultiProcessor[i]);
			}
		}
	}
	Rand = 1 + rand() % 100;
	if (Rand < 10) {
		Process* Curr = nullptr;
		if (BLK.dequeue(Curr)) {
			TO_RDY(Curr, Turn);
		}
	}
	return true;
}
void Scheduler::TO_BLK(Process* P)
{
	P->SetState(BLk);
	BLK.enqueue(P);
}
void Scheduler::TO_TRM(Process* P)
{
	P->SetState(TRm);
	TRM.enqueue(P);
}
void Scheduler::RET_TO_RDY(Process* P) {
	P->SetState(RDy);
	Processor* processor = P->GetProcessor();
	processor->AddProcess(P);
	P->SetProcessor(nullptr);
}
void Scheduler::TO_RDY(Process* P, int& i)
{
	P->SetState(RDy);
	MultiProcessor[i]->AddProcess(P);
	i = (i + 1) % Num_of_Processors;
}
void Scheduler::SchedulerUpdater(Processor* P) {
	P->UpdateState();
	if (P->Get_Run())
		P->Get_Run()->SetProcessor(nullptr);
}
void Scheduler::UpdateInterface() const
{
	if (Mode == Silent)
	{
		if (TimeStep == 1)
			pOut->PrintOut("Silent Mode............ Simulation Starts..........\n");
		if (TRM.GetSize() == M)
			pOut->PrintOut("Simulation ends, Output file created\n");
	}
	else {
		pOut->PrintInfo(MultiProcessor, Num_of_Processors, BLK, TRM, TimeStep);
		if (Mode == Interactive) {
			pOut->PrintOut("PRESS ANY KEY TO MOVE TO NEXT STEP!\n"); 
			getc(stdin); 
		}
		else Sleep(1000);
		if (TRM.GetSize() != M)
			system("CLS");
	}
}