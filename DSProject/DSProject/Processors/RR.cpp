#include "RR.h"
#include "../Process Scheduler/Process Scheduler.h"


RR::RR(Scheduler* Sched, int tsr,int n)
	:Processor(Sched,n), TSR(tsr),TSRTemp(tsr) {}
void RR::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.dequeue(R)) {

		R->AddWaitingTime(S->Get_TimeStep() - R->GetLastRunTime());  // This Line and the next one should be added to all processors  
		
		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());

		R->SetState(RUn);

		State = BUSY;

		QFT -= R->GetCPURemainingTime();

		RRMigration();
	}
	if (State == BUSY) {

		TSRTemp--;

		TBT++;

		R->UpdateInfo();

		if (!R->GetCPURemainingTime())
			S->TO_TRM(R);

		else if (R->GetIO() && !R->GetIO()->getFirst())
			S->TO_BLK(R);

		else if(!TSRTemp)
			Round();
	}
	else if (State == STOP) {
		if (N_TEMP == N) {
			if (R) {
				R->SetProcessor(nullptr);
				S->TO_SHORTEST_RDY(R);
			}
			while (RDY_LIST.dequeue(R))
				S->TO_SHORTEST_RDY(R);
			QFT = 0;
			R = nullptr;
		}
		else if (!N_TEMP) {
			State = IDLE;
			N_TEMP = N;
			return;
		}
		N_TEMP--;
	}
	else TIT++;
}


void RR::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.enqueue(process);
	QFT += process->GetCPURemainingTime();
}

void RR::Round() {
	R->SetState(RDy);
	AddProcess(R);
	TSRTemp = TSR;
}

void RR::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[RR]: " + to_string(RDY_LIST.GetSize()) + " RDY: ");
	RDY_LIST.Print();
}

void RR::Lose(Process*& Stolen) {
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}

void RR::RRMigration() {

	if (S->Get_NS()) { //Don't enter if no SJF exists 
		while (R->GetCPURemainingTime() < S->Get_RTF()) { //Migrate Multiple Processes in the same time step until a process has Remaining Time less than RTF
			
			S->RRMigration(R);

			if (RDY_LIST.dequeue(R)) {
				QFT -= R->GetCPURemainingTime();
				if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
				R->SetProcessor(this);
				R->SetState(RUn);
				State = BUSY;
			}
			else break;
		}
	}
}