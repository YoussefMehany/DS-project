#include "Process Scheduler.h"
#include "../Processors/Processor.h"
#include "../Processors/FCFS.h"
#include "../Processors/RR.h"
#include "../Processors/SJF.h"
#include <windows.h>
Scheduler::Scheduler() {
	TimeStep = NS = NF = NR = Num_of_Processors = TTAT = RTF = M = MaxW = STL = Fork_Prob = Turn = 0;
	MultiProcessor = nullptr;
	IDs = nullptr;
	LQ = SQ = nullptr;
	pIn = new Input;
	pOut = new Output;
	Mode = Interactive;
	srand(time(0));
}
void Scheduler::Set_Mode(InterfaceMode mode) {
	Mode = mode;
}
void Scheduler::AddProcessors(int FCFScnt, int SJFcnt, int RRcnt, int TSR) {
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

void Scheduler::WriteData() {
	pOut->PrintOut("Enter a name for output file : ");
	pIn->GetFileName(Filename);
	OutFile.open(Filename + ".txt");
	OutFile << "TT\tPID\tAT\tCT\tIO_D\tWT\tRT\tTRT\n";
	Process* p = nullptr;
	Queue<Process*> TRMT = TRM;
	int AvgResponseTime = 0, AvgWaitingTime = 0, AvgTurnAroundTime = 0;
	double AvgUtil = 0;
	while (TRMT.dequeue(p)) {
		p->PrintOutFile(OutFile);
		AvgResponseTime += p->GetResponseTime();
		AvgWaitingTime += p->GetWaitingTime();
		AvgTurnAroundTime += p->GetTurnAroundDuration();
	}
	AvgResponseTime /= TRM.GetSize();
	AvgWaitingTime /= TRM.GetSize();
	AvgTurnAroundTime /= TRM.GetSize();
	OutFile << "Processes : " << TRM.GetSize() << '\n'
		<< "Avg WT = " << AvgWaitingTime << ",\t\tAvg RT = " << AvgResponseTime <<
		",\t\tAvgTRT = " << AvgTurnAroundTime << '\n';
	//////// Migration Calculations Remained

	OutFile << "Processors: " << Num_of_Processors << " [" << NF << " FCFS, "
		<< NS << " SJF, " << NR << " RR]\n";
	OutFile << "Processors Load\n";
	for (int i = 0; i < Num_of_Processors; i++) {
		OutFile << "p" << i + 1 << " = " << MultiProcessor[i]->Get_pLoad() * 100 << "%";
		if (i != Num_of_Processors - 1)
			OutFile << ",\t";
	}
	OutFile << "\n\nProcessors Utiliz\n";
	for (int i = 0; i < Num_of_Processors; i++) {
		OutFile << "p" << i + 1 << " = " << MultiProcessor[i]->Get_pUtil() * 100 << "%";
		AvgUtil += MultiProcessor[i]->Get_pUtil();
		if (i != Num_of_Processors - 1)
			OutFile << ",\t";
	}
	AvgUtil /= Num_of_Processors;
	OutFile << "Avg Utilization = " << AvgUtil * 100 << "%\n";
}

void Scheduler::Get_Data() {
	int TSR = 0;
	pIn->GetFileName(Filename);
	InFile.open(Filename + ".txt");
	InFile >> NF >> NS >> NR >> TSR >> RTF >> MaxW >> STL >> Fork_Prob >> M;
	IDs = new int[M];
	for (int i = 0; i < M; i++) {
		int AT, ID, CPU, N, IO_R, IO_D, sum_IOD = 0;
		InFile >> AT >> ID >> CPU >> N;
		Process* process = new Process(AT, CPU, ID);
		IDs[i] = ID;
		while (N--) {
			char dummy;
			InFile >> dummy >> IO_R >> dummy >> IO_D >> dummy;
			sum_IOD += IO_D;
			if (N) InFile >> dummy;
			process->AddIO(IO_R, IO_D);
		}
		process->SetTIOD(sum_IOD);
		NEW.enqueue(process);
	}
	AddProcessors(NF, NS, NR, TSR);
	int T, PID;
	while (InFile >> T) {
		InFile >> PID;
		Pair<int, int>* kill = new Pair<int, int>(T, PID);
		KILLSIG.enqueue(kill);
	}
	system("CLS");
	int x = 1;
	pOut->PrintOut("Please Enter The Mode of The Interface :\n");
	pOut->PrintOut("1.Interactive Mode\n2.Step-By-Step Mode\n3.Silent Mode\n");
	pIn->GetInput(x);
	Mode = InterfaceMode(x - 1);
	system("CLS");
}

bool Scheduler::Simulation() {

	TimeStep++;

	if (TRM.GetSize() == M)
		return false;

	while (!NEW.isEmpty()) {
		Process* temp = nullptr;
		NEW.peek(temp);
		if (TimeStep == temp->GetArrivalTime()) {
			NEW.dequeue(temp);
			TO_SHORTEST_RDY(temp);
		}
		else break;
	}


	////////////////////////////////// KILLSIG //////////////////////////////////////

	Pair<int, int>* Kill = nullptr;
	KILLSIG.peek(Kill);
	if (Kill && Kill->getFirst() == TimeStep) {
		KILLSIG.dequeue(Kill);
		for (int i = 0; i < Num_of_Processors; i++) {
			if (dynamic_cast<FCFS*>(MultiProcessor[i]))  //KILL From FCFS Only , else Ignore the Signal
				((FCFS*)MultiProcessor[i])->Kill(Kill->getSecond());
		}
	}

	///////////////////////////////// Forking /////////////////////////////////////

	for (int i = 0; i < Num_of_Processors; i++) {
		if (dynamic_cast<FCFS*>(MultiProcessor[i]))  //Check Fork_Prob and add children
			((FCFS*)MultiProcessor[i])->Forking();
	}


	/////////////////////////////////// WORK STEALING ///////////////////////////////

	if (!(TimeStep % STL) && Num_of_Processors)
		WorkStealing();

	/////////////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < Num_of_Processors; i++) { //Move process from RDY to RUN if processor is IDLE
		MultiProcessor[i]->ScheduleAlgo();
	}

	/////////////////////////////////// Perform IO tasks ///////////////////////////////

	Process* Blocked = nullptr;
	if(BLK.peek(Blocked)) {
		if (!MakeIO(Blocked)) {
			BLK.dequeue(Blocked);
			Blocked->NextIO();
			TO_SHORTEST_RDY(Blocked);
		}
	}

	/////////////////////////////////// Remove Orphans ///////////////////////////////




	for (int i = 0; i < Num_of_Processors; i++) {
		if (dynamic_cast<FCFS*>(MultiProcessor[i])) {
			((FCFS*)MultiProcessor[i])->RemoveOrphans();
		}
	}

	return true;
}

/////////////////////////////////// FCSF->RR Migration////////////////////////////
int Scheduler::Get_MaxW()const {
	return MaxW;
}
int Scheduler::Get_RTF()const {
	return RTF;
}
int Scheduler::Get_FP() const {
	return Fork_Prob;
}
int Scheduler::Get_NR()  const {
	return NR;
}
int Scheduler::Get_NS()  const {
	return NS;
}
void Scheduler::FCFSMigration(Process* Migrate) {
	Migrate->SetState(RDy);
	Migrate->GetProcessor()->UpdateState();
	Migrate->SetProcessor(nullptr);
	Processor* RRSQ = nullptr; //RR Shortest Ready Queue
	for (int i = 0; i < Num_of_Processors; i++) {
		if (dynamic_cast<RR*>(MultiProcessor[i]))
			if (!RRSQ)
				RRSQ = MultiProcessor[i];
			else if (RRSQ->GET_QFT() > MultiProcessor[i]->GET_QFT())
				RRSQ = MultiProcessor[i];
	}
	RRSQ->AddProcess(Migrate);
}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// RR->SJF Migration ////////////////////////////////

void Scheduler::RRMigration(Process* Migrate) {
	Migrate->SetState(RDy);
	Migrate->GetProcessor()->UpdateState();
	Migrate->SetProcessor(nullptr);
	Processor* SJFSQ = nullptr; //SJF Shortest Ready Queue
	for (int i = 0; i < Num_of_Processors; i++) {
		if (dynamic_cast<SJF*>(MultiProcessor[i]))
			if (!SJFSQ)
				SJFSQ = MultiProcessor[i];
			else if (SJFSQ->GET_QFT() > MultiProcessor[i]->GET_QFT())
				SJFSQ = MultiProcessor[i];
	}
	SJFSQ->AddProcess(Migrate);
}

//////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////// WORK STEALING ////////////////////////////////
Processor* Scheduler::GetSQ() const {
	return SQ;
}
void Scheduler::WorkStealing() {

	if (!LQ) LQ = MultiProcessor[0];
	if (!SQ) SQ = MultiProcessor[0];

	for (int i = 0; i < Num_of_Processors; i++) {
		if (LQ->GET_QFT() < MultiProcessor[i]->GET_QFT()) LQ = MultiProcessor[i];
		if (SQ->GET_QFT() > MultiProcessor[i]->GET_QFT()) SQ = MultiProcessor[i];
	}

	Process* Stolen = nullptr;
	int LQF = LQ->GET_QFT(), SQF = SQ->GET_QFT();
	double StealLimit = double(LQF - SQF) / LQF;

	while (LQF > 0 && StealLimit > 0.4)
	{
		LQ->Lose(Stolen);
		if (!Stolen) break;
		SQ->AddProcess(Stolen);
		LQF = LQ->GET_QFT();
		SQF = SQ->GET_QFT();
		StealLimit = double(LQF - SQF) / LQF;
	}
}

/////////////////////////////IO_TASKS//////////////////////////////////////
bool Scheduler::MakeIO(Process* Blocked) {
	int Remaining = Blocked->GetIO()->getSecond();
	if (Remaining) {
		Blocked->GetIO()->SetSecond(--Remaining);
	}
	return Remaining;
}


void Scheduler::DecideShortest() {
	int CPU_Min = INT_MAX;
	int index = 0;
	if (Num_of_Processors) {
		for (int i = 0; i < Num_of_Processors; i++) {
			int QFT = MultiProcessor[i]->GET_QFT();
			if (QFT < CPU_Min) {
				index = i;
				CPU_Min = QFT;
			}
		}
		SQ = MultiProcessor[index];
	}
}

Processor* Scheduler::DecideShortestFCFS() {
	int CPU_Min = INT_MAX;
	int index = 0;
	for (int i = 0; i < Num_of_Processors; i++) {
		int QFT = INT_MAX;
		if (dynamic_cast<FCFS*>(MultiProcessor[i]))
			QFT = MultiProcessor[i]->GET_QFT();
		if (QFT < CPU_Min) {
			index = i;
			CPU_Min = QFT;
		}
	}
	return MultiProcessor[index];
}

Process* Scheduler::AddChildToSQ(int ArrivalT, int RemCPU) {
	Process* child = new Process(ArrivalT, RemCPU);
	DecideShortestFCFS()->AddProcess(child);
	M++;
	return child;
}

//////////////////////////////////////////////////////////////////////////////////
void Scheduler::TO_BLK(Process* P) {
	P->SetState(BLk);
	P->SetLastRunTime(TimeStep);
	P->GetProcessor()->UpdateState();
	P->SetProcessor(nullptr);
	BLK.enqueue(P);
}
void Scheduler::TO_TRM(Process* P) {
	P->SetTerminationTime(TimeStep);
	P->SetLastRunTime(TimeStep);
	P->SetState(TRm);
	P->GetProcessor()->UpdateState();
	P->SetProcessor(nullptr);
	TRM.enqueue(P);
}

void Scheduler::TO_RDY(Process* P, int& i) {
	P->SetState(RDy);
	MultiProcessor[i]->AddProcess(P);
	i = (i + 1) % Num_of_Processors;
}
void Scheduler::TO_SHORTEST_RDY(Process* P) {
	P->SetState(RDy);
	DecideShortest();
	SQ->AddProcess(P);
}
void Scheduler::SchedulerUpdater(Processor* P) {
	P->UpdateState();
	if (P->Get_Run())
		P->Get_Run()->SetProcessor(nullptr);
}
void Scheduler::UpdateInterface() {

	if (Mode == Silent)
	{
		if (TimeStep == 1)
			pOut->PrintOut("Silent Mode............ Simulation Starts..........\n");
		if (TRM.GetSize() == M)
			pOut->PrintOut("Simulation ends, Output file created\n");
	}
	else {
		cout << M << endl;
		pOut->PrintInfo(MultiProcessor, Num_of_Processors, BLK, TRM, TimeStep);
		if (Mode == Interactive) {
			pOut->PrintOut("PRESS ANY KEY TO MOVE TO NEXT STEP!\n");
			getc(stdin);
		}
		else Sleep(250);
		if (TRM.GetSize() != M)
			system("CLS");
	}
}
int Scheduler::Get_TimeStep() {
	return TimeStep;
}
Input* Scheduler::getInput() {
	return pIn;
}
Output* Scheduler::getOutput() {
	return pOut;
}
Scheduler::~Scheduler() {
	delete IDs;
	delete pIn;
	delete pOut;
	Process* p = nullptr;
	while (TRM.dequeue(p)) {
		delete p;
	}
	Pair<int, int>* k = nullptr;
	while (KILLSIG.dequeue(k)) {
		delete k;
	}
	for (int i = 0; i < Num_of_Processors; i++) {
		delete MultiProcessor[i];
	}
	delete MultiProcessor;
}