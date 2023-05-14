#include "Process Scheduler.h"
#include "../Processors/Processor.h"
#include "../Processors/FCFS.h"
#include "../Processors/RR.h"
#include "../Processors/SJF.h"
#include "../Processors/EDF.h"
#include <iomanip>
#include <windows.h>

Scheduler::Scheduler() {
	TimeStep = NS = NF = NR = ND = Num_of_Processors = TTAT = RTF = M = MaxW = STL = Fork_Prob = ProcessesMaxW = ProcessesRTF = ProcessesStolen = n = 0;
	Heat_Prob = 2;
	MultiProcessor = nullptr;
	LQ = SQ = SFCFS = SSJF = SRR = SEDF = nullptr;
	pIn = new Input;
	pOut = new Output;
	Mode = Interactive;
	srand(time(0));
}
void Scheduler::Set_Mode(InterfaceMode mode) {
	Mode = mode;
}
void Scheduler::AddProcessors(int FCFScnt, int SJFcnt, int RRcnt, int EDFcnt, int TSR, int n) {
	Num_of_Processors = FCFScnt + SJFcnt + RRcnt + EDFcnt;
	MultiProcessor = new Processor * [Num_of_Processors];
	int cnt = 0;
	while (FCFScnt--) {
		MultiProcessor[cnt++] = new FCFS(this, n);
	}
	while (SJFcnt--) {
		MultiProcessor[cnt++] = new SJF(this, n);
	}
	while (RRcnt--) {
		MultiProcessor[cnt++] = new RR(this, TSR, n);
	}
	while (EDFcnt--) {
		MultiProcessor[cnt++] = new EDF(this, n);
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
	int Early = 0; 
	double AvgUtil = 0;
	while (TRMT.dequeue(p)) {
		p->PrintOutFile(OutFile);
		AvgResponseTime += p->GetResponseTime();
		AvgWaitingTime += p->GetWaitingTime();
		AvgTurnAroundTime += p->GetTurnAroundDuration();
		if (p->GetTerminationTime() < p->GetDeadLine())
			Early++;
	}
	AvgResponseTime /= TRM.GetSize();
	AvgWaitingTime /= TRM.GetSize();
	AvgTurnAroundTime /= TRM.GetSize();
	OutFile << "Processes : " << TRM.GetSize() << '\n'
		<< "Avg WT = " << AvgWaitingTime << ",\t\tAvg RT = " << AvgResponseTime <<
		",\t\tAvgTRT = " << AvgTurnAroundTime << '\n';
	double forked_per = 100 * (double)(M - INIT_M) / M;
	OutFile << fixed << setprecision(3) << "Migration %:\t\tRTF= " << 100.0 * ProcessesRTF / M <<
		"%,\t\tMaxW = " << 100.0 * ProcessesMaxW / M <<
		"%\nWork Steal: " << 100.0 * ProcessesStolen / M << "%\nForked Process: " << forked_per <<
		"\nEarly Completed: " << 100.0 * Early / M;



	OutFile << "%\nProcessors: " << Num_of_Processors << " [" << NF << " FCFS, "
		<< NS << " SJF, " << NR << " RR]\n";
	OutFile << "Processors Load\n";
	for (int i = 0; i < Num_of_Processors; i++) {
		OutFile << "p" << i + 1 << " = " << MultiProcessor[i]->Get_pLoad() << "%";
		if (i != Num_of_Processors - 1)
			OutFile << ",\t";
	}
	OutFile << "\n\nProcessors Utilization\n";
	for (int i = 0; i < Num_of_Processors; i++) {
		OutFile << "p" << i + 1 << " = " << MultiProcessor[i]->Get_pUtil() << "%";
		AvgUtil += MultiProcessor[i]->Get_pUtil();
		if (i != Num_of_Processors - 1)
			OutFile << ",\t";
	}
	AvgUtil /= Num_of_Processors;
	OutFile << "\nAvg Utilization = " << AvgUtil << "%\n";
	pOut->PrintOut("File has been created , check it please.");
}

void Scheduler::Get_Data() {
	int TSR = 0;
	pOut->PrintOut("Enter File name: ");
	pIn->GetFileName(Filename);
	pOut->ClearConsole();
	InFile.open(Filename + ".txt");
	pOut->PrintOut("Processing Input Data...\n");
	Sleep(500);
	InFile >> NF >> NS >> NR >> ND >> TSR >> RTF >> MaxW >> STL >> Fork_Prob >> n >> INIT_M;
	M = INIT_M;
	for (int i = 0; i < INIT_M; i++) {
		int AT, ID, CPU, N_IO, IO_R, IO_D, sum_IOD = 0, DeadLine;
		InFile >> AT >> ID >> CPU >> DeadLine >> N_IO;
		Process* process = new Process(AT, CPU, DeadLine, ID);
		while (N_IO--) {
			char dummy;
			InFile >> dummy >> IO_R >> dummy >> IO_D >> dummy;
			sum_IOD += IO_D;
			if (N_IO) InFile >> dummy;
			process->AddIO(IO_R, IO_D);
		}
		process->SetTIOD(sum_IOD);
		NEW.enqueue(process);
	}
	AddProcessors(NF, NS, NR, ND, TSR, n);
	int T, PID;
	while (InFile >> T) {
		InFile >> PID;
		Pair<int, int>* kill = new Pair<int, int>(T, PID);
		KILLSIG.enqueue(kill);
	}
	pOut->ClearConsole();
	int x = 1;
	pOut->PrintOut("Please Enter The Mode of The Interface :\n");
	pOut->PrintOut("1.Interactive Mode\n2.Step-By-Step Mode\n3.Silent Mode\n");
	pIn->GetInput(x);
	Mode = InterfaceMode(x - 1);
	pOut->ClearConsole();
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

	/////////////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < Num_of_Processors; i++) { //Move process from RDY to RUN if processor is IDLE
		int Overheat_Prob = 1 + rand() % 100;
		if (Overheat_Prob <= Heat_Prob) {
			MultiProcessor[i]->SetProcessorState(STOP);
		}
		MultiProcessor[i]->ScheduleAlgo();
	}

	/////////////////////////////////// Perform IO tasks ///////////////////////////////

	Process* Blocked = nullptr;
	if (BLK.peek(Blocked)) {
		if (!MakeIO(Blocked)) {
			BLK.dequeue(Blocked);
			Blocked->NextIO();
			TO_SHORTEST_RDY(Blocked);
		}
	}

	/////////////////////////////////// WORK STEALING ///////////////////////////////

	if (!(TimeStep % STL) && Num_of_Processors)
		WorkStealing();

	/////////////////////////////////// Remove Orphans ///////////////////////////////


	for (int i = 0; i < Num_of_Processors; i++) {
		if (dynamic_cast<FCFS*>(MultiProcessor[i])) {
			((FCFS*)MultiProcessor[i])->RemoveOrphans();
		}
	}

	///////////////////////////////// Forking /////////////////////////////////////

	for (int i = 0; i < Num_of_Processors; i++) {
		if (dynamic_cast<FCFS*>(MultiProcessor[i]))  //Check Fork_Prob and add children
			((FCFS*)MultiProcessor[i])->Forking();
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
	SRR->AddProcess(Migrate);
	ProcessesMaxW++;
}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// RR->SJF Migration ////////////////////////////////

void Scheduler::RRMigration(Process* Migrate) {
	Migrate->SetState(RDy);
	Migrate->GetProcessor()->UpdateState();
	Migrate->SetProcessor(nullptr);
	SSJF->AddProcess(Migrate);
	ProcessesRTF++;
}

//////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////// WORK STEALING ////////////////////////////////
Processor* Scheduler::GetSQ() const {
	return SQ;
}
void Scheduler::WorkStealing() {

	DecideLongest();
	DecideShortest(4);

	Process* Stolen = nullptr;
	if (!LQ) return;
	int LQF = LQ->GET_QFT(), SQF = SQ->GET_QFT();
	double StealLimit = double(LQF - SQF) / LQF;
	while (LQF > 0 && StealLimit > 0.4)
	{
		LQ->Lose(Stolen);
		if (!Stolen) break;
		SQ->AddProcess(Stolen);
		Stolen = nullptr;
		LQF = LQ->GET_QFT();
		SQF = SQ->GET_QFT();
		StealLimit = double(LQF - SQF) / LQF;
		ProcessesStolen++;
	}
}

/////////////////////////////IO_TASKS//////////////////////////////////////
bool Scheduler::MakeIO(Process* Blocked) {
	int Remaining = Blocked->GetIO()->getSecond();
	if (Remaining)
		Blocked->GetIO()->SetSecond(--Remaining);

	return Remaining;
}

void Scheduler::DecideLongest() {
	int CPU_Max = -1;
	int index = -1;
	LQ = nullptr;
	for (int i = 0; i < Num_of_Processors; i++) {
		if (MultiProcessor[i]->Get_State() != STOP) {
			int QFT = MultiProcessor[i]->GET_QFT();
			if (QFT > CPU_Max) {
				index = i;
				CPU_Max = QFT;
			}
		}
	}
	if (~index)
		LQ = MultiProcessor[index];
}


void Scheduler::DecideShortest(int Type) { //decide the shortest queue of a specific type depending on the argument sent
	//Types: FCFS: 0, SJF: 1, RR: 2, EDF: 3 ,SQ : 4
	int CPU_Min = INT_MAX;

	(Type == 0 ? SFCFS : Type == 1 ? SSJF : Type == 2 ? SRR : Type == 3? SEDF : SQ) = nullptr;
	int index = -1;
	for (int i = 0; i < Num_of_Processors; i++) {
		int QFT = INT_MAX;
		Processor* dynamic = nullptr;
		if (Type == 0) {
			dynamic = dynamic_cast<FCFS*>(MultiProcessor[i]);
		}
		else if (Type == 1) {
			dynamic = dynamic_cast<SJF*>(MultiProcessor[i]);
		}
		else if (Type == 2) {
			dynamic = dynamic_cast<RR*>(MultiProcessor[i]);
		}
		else {
			dynamic = dynamic_cast<EDF*>(MultiProcessor[i]);
		}
		if ((dynamic || Type == 4) && MultiProcessor[i]->Get_State() != STOP)
			QFT = MultiProcessor[i]->GET_QFT();
		if (QFT < CPU_Min) {
			index = i;
			CPU_Min = QFT;
		}
	}
	if (~index)
		(Type == 0 ? SFCFS : Type == 1 ? SSJF : Type == 2 ? SRR : Type == 3 ? SEDF : SQ) = MultiProcessor[index];
}

Process* Scheduler::AddChildToSQ(int RemCPU,int DeadLine) {
	Process* child = new Process(TimeStep, RemCPU, DeadLine);
	CoolingSystem(true);
	DecideShortest(0);
	SFCFS->AddProcess(child);
	M++;
	return child;
}

//////////////////////////////////////////////////////////////////////////////////
void Scheduler::TO_BLK(Process* P) {
	P->SetState(BLk);
	P->GetProcessor()->UpdateState();
	P->SetProcessor(nullptr);
	BLK.enqueue(P);
}
void Scheduler::TO_TRM(Process* P) {
	P->SetTerminationTime(TimeStep);
	P->SetTurnAroundDuration();
	P->SetWaitingTime();
	P->SetState(TRm);
	P->GetProcessor()->UpdateState();
	P->SetProcessor(nullptr);
	TRM.enqueue(P);
}
void Scheduler::CoolingSystem(bool fcfs) {
	Processor* Coolest = nullptr;
	int OverHeated = 0;
	int MinHeatFactor = n + 1;
	for (int i = 0; i < Num_of_Processors; i++) {
		bool Stop = MultiProcessor[i]->Get_State() == STOP;
		if (fcfs && dynamic_cast<FCFS*>(MultiProcessor[i]) && Stop || (!fcfs && Stop)) {
			if (MultiProcessor[i]->Get_HeatFactor() < MinHeatFactor) {
				MinHeatFactor = MultiProcessor[i]->Get_HeatFactor();
				Coolest = MultiProcessor[i];
			}
			OverHeated++;
		}
	}
	if (OverHeated == (fcfs ? NF : Num_of_Processors)) {
		Coolest->SetProcessorState(IDLE);
		Coolest->Reset_HeatFactor();
	}
}

void Scheduler::TO_SHORTEST_RDY(Process* P, bool fcfs) {
	P->SetState(RDy);
	if (!fcfs) {
		CoolingSystem();   ///Check if all processors are in STOP state, then cool the least heated one
		DecideShortest(4);
		SQ->AddProcess(P);
	}
	else {
		CoolingSystem(true); ///Check if all processors(FCFS) are in STOP state, then cool the least heated one
		DecideShortest(0);
		SFCFS->AddProcess(P);
	}
}

void Scheduler::UpdateInterface() {

	if (TRM.GetSize() == M) {
		pOut->PrintOut("Simulation ended, Output file is created\n");
		return;
	}

	if (Mode == Silent)
	{
		if (TimeStep == 1)
			pOut->PrintOut("Silent Mode............ Simulation Starts..........\n");
	}
	else {
		pOut->PrintInfo(MultiProcessor, Num_of_Processors, BLK, TRM, TimeStep);
		if (Mode == Interactive) {
			pOut->PrintOut("PRESS ANY KEY TO MOVE TO NEXT STEP!\n");
			getc(stdin);
		}
		else Sleep(50);
		/*if (TRM.GetSize() != M)
			pOut->ClearConsole();*/
	}
}
int Scheduler::Get_TimeStep() {
	return TimeStep;
}
Processor* Scheduler::GetSFCFS() const {
	return SFCFS;
}
Processor* Scheduler::GetSSJF() const {
	return SSJF;
}
Processor* Scheduler::GetSRR() const {
	return SRR;
}
Input* Scheduler::getInput() {
	return pIn;
}
Output* Scheduler::getOutput() {
	return pOut;
}
Scheduler::~Scheduler() {
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