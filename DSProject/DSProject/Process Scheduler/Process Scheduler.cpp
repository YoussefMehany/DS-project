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
void Scheduler::Get_Data() {
	int TSR = 0;
	pIn->GetFileName(Filename);
	InFile.open(Filename + ".txt");
	InFile >> NF >> NS >> NR >> TSR >> RTF >> MaxW >> STL >> Fork_Prob >> M;
	IDs = new int[M];
	for (int i = 0; i < M; i++) {
		int AT, ID, CPU, N, IO_R, IO_D;
		InFile >> AT >> ID >> CPU >> N;
		Process* process = new Process(AT, ID, CPU);
		IDs[i] = ID;
		while (N--) {
			char dummy;
			InFile >> dummy >> IO_R >> dummy >> IO_D >> dummy;
			if (N) InFile >> dummy;
			process->AddIO(IO_R, IO_D);
		}
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

	if (TRM.GetSize() + BLK.GetSize() == M) // Have to be modified when BLK operations are Done
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

	/////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////// WORK STEALING ///////////////////////////////

	if (!(TimeStep % STL))
		WorkStealing();

	/////////////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < Num_of_Processors; i++) { //Move process from RDY to RUN if processor is IDLE
		MultiProcessor[i]->ScheduleAlgo();
	}


	//int Rand = 0;
	//for (int i = 0; i < Num_of_Processors; i++) {
	//	if (MultiProcessor[i]->Get_State() == BUSY) {
	//		Rand = 1 + rand() % 100;
	//		Process* process = MultiProcessor[i]->Get_Run();
	//		if (Rand <= 15) {
	//			TO_BLK(process);
	//			SchedulerUpdater(MultiProcessor[i]);
	//		}
	//		else if (Rand >= 20 && Rand <= 30) {
	//			RET_TO_RDY(process);
	//		}
	//		else if (Rand >= 50 && Rand <= 60) {
	//			TO_TRM(process);
	//			SchedulerUpdater(MultiProcessor[i]);
	//		}
	//	}
	//}

	//Rand = 1 + rand() % 100;
	//if (Rand < 10) {
	//	Process* Curr = nullptr;
	//	if (BLK.peek(Curr)) {
	//		BLK.dequeue(Curr);
	//		TO_RDY(Curr, Turn);
	//	}
	//}
	//Rand = rand() % M;
	//for (int i = 0; i < Num_of_Processors; i++) {    //KILL Random process from FCFS
	//	if (dynamic_cast<FCFS*>(MultiProcessor[i]))
	//		((FCFS*)MultiProcessor[i])->Kill(IDs[Rand]);
	//}

	return true;
}

/////////////////////////////////// FCSF->RR Migration////////////////////////////
int Scheduler::Get_MaxW()const {
	return MaxW;
}
int Scheduler::Get_NR()  const {
	return NR;
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
	if (RRSQ) RRSQ->AddProcess(Migrate);
}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// WORK STEALING ////////////////////////////////

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
		if (Stolen)
			SQ->AddProcess(Stolen);
		LQF = LQ->GET_QFT();
		SQF = SQ->GET_QFT();
		StealLimit = double(LQF - SQF) / LQF;
	}
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
	P->SetState(TRm);
	P->GetProcessor()->UpdateState();
	P->SetProcessor(nullptr);
	TRM.enqueue(P);
}
void Scheduler::RET_TO_RDY(Process* P) {
	P->SetState(RDy);
	Processor* processor = P->GetProcessor();
	processor->AddProcess(P);
}
void Scheduler::TO_RDY(Process* P, int& i) {
	P->SetState(RDy);
	MultiProcessor[i]->AddProcess(P);
	i = (i + 1) % Num_of_Processors;
}
void Scheduler::SchedulerUpdater(Processor* P) {
	P->UpdateState();
	if (P->Get_Run())
		P->Get_Run()->SetProcessor(nullptr);
}
void Scheduler::UpdateInterface() {
	//cout << BLK.GetSize() << endl;
	if (Mode == Silent)
	{
		if (TimeStep == 1)
			pOut->PrintOut("Silent Mode............ Simulation Starts..........\n");
		if (TRM.GetSize() + BLK.GetSize() == M)  // Have to be modified when BLK operations are Done
			pOut->PrintOut("Simulation ends, Output file created\n");
	}
	else {
		pOut->PrintInfo(MultiProcessor, Num_of_Processors, BLK, TRM, TimeStep);
		if (Mode == Interactive) {
			pOut->PrintOut("PRESS ANY KEY TO MOVE TO NEXT STEP!\n");
			getc(stdin);
		}
		else Sleep(100);
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