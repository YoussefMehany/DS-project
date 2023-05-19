#include "RR.h"
#include "../Process Scheduler/Process Scheduler.h"


RR::RR(Scheduler* Sched, int tsr,int n)
	:Processor(Sched,n), TSR(tsr),TSRTemp(tsr) {}
void RR::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.dequeue(R)) {
		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep()); //Response time setting
		AddToRun();
		RRMigration();
	}
	if (State == BUSY) {
		RunProcess();
	}
	else if (State == STOP) { //Overheat state
		OverHeat();
	}
	else TIT++;
}


void RR::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.enqueue(process);
	QFT += process->GetCPURemainingTime();
}
void RR::RunProcess() {
	TSRTemp--;
	TBT++;
	R->UpdateInfo();
	if (!R->GetCPURemainingTime())
		S->TO_TRM(R);
	else if (R->GetIO() && !R->GetIO()->getFirst()) //If the IO duration is zero, then go to BLK
		S->TO_BLK(R);
	else if (!TSRTemp) //Time Slice
		Round();
}

void RR::OverHeat() {
	if (N_TEMP == N) {
		if (R) {
			R->SetProcessor(nullptr);
			S->TO_SHORTEST_RDY(R);
		}
		int Size = RDY_LIST.GetSize();
		while (Size--) {
			RDY_LIST.dequeue(R);
			R->SetProcessor(nullptr);
			S->TO_SHORTEST_RDY(R);
		}
		QFT = 0;
		R = nullptr;
	}
	else if (!N_TEMP) { //if cooldown ended, then reset heat factor and go back to idle state
		State = IDLE;
		N_TEMP = N;
		return;
	}
	if (State == STOP) N_TEMP--;
	else N_TEMP = N;
}

void RR::Round() {
	R->SetState(RDy);
	AddProcess(R);
	TSRTemp = TSR;
}

void RR::Print() {
	Output* pOut = S->getOutput();
	Colors color = State == STOP ? RED : State == BUSY ? GREEN : GREY;
	pOut->PrintOut("Processor " + to_string(ID), color);
	pOut->PrintOut("[RR]: " + to_string(RDY_LIST.GetSize()) + " RDY: ", color);
	pOut->PrintColor(color);
	RDY_LIST.Print();
}

void RR::Lose(Process*& Stolen) { //work stealing function
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}

void RR::RRMigration() {

	if (S->Get_NS()) { //Don't enter if no SJF exists 
		S->DecideShortest(1); //Check that there is a non-heated Processor of type SJF to move too, and return if there isnt
		if (!S->GetSSJF()) return;
		while (R->GetCPURemainingTime() < S->Get_RTF()) { //Migrate Multiple Processes in the same time step until a process has Remaining Time less than RTF		
			S->RRMigration(R);
			if (RDY_LIST.dequeue(R)) {
				QFT -= R->GetCPURemainingTime();
				if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
				R->SetState(RUn);
				State = BUSY;
			}
			else break;
		}
	}
}
