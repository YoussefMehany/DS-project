#include "RR.h"
#include "../Process Scheduler/Process Scheduler.h"


RR::RR(Scheduler* Sched, int tsr)
	:Processor(Sched), TSR(tsr),TSRTemp(tsr) {}
void RR::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.dequeue(R)) {
		R->AddWaitingTime(S->Get_TimeStep() - R->GetLastRunTime());  // This Line and the next one should be added to all processors  
		R->SetLastRunTime(S->Get_TimeStep());
		State = BUSY;
		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
		R->SetState(RUn);
		R->SetProcessor(this);
		QFT -= min(R->GetCPURemainingTime(),TSR);
		RRMigration();
	}
	if (State == BUSY) {
		TSRTemp--;
		TBT++;
		R->UpdateInfo();
		if (!R->GetCPURemainingTime()) {
			S->TO_TRM(R);
		}
		else if (R->GetIO() && !R->GetIO()->getFirst())
			S->TO_BLK(R);
		else if(!TSRTemp) {
			RDY_LIST.enqueue(R);
			TSRTemp = TSR;
			RDY_LIST.peek(R);
			UpdateState();
		}
	}
	else TIT++;
}
void RR::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.enqueue(process);
}
void RR::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[RR]: " + to_string(RDY_LIST.GetSize()) + " RDY: ");
	RDY_LIST.Print();
}
void RR::Lose(Process*& Stolen) {
	if (!RDY_LIST.dequeue(Stolen)) Stolen = nullptr;
	else QFT -= Stolen->GetCPUTime();
}

void RR::RRMigration() {

	if (S->Get_NS()) { //Don't enter if no SJF exists 
		while (R->GetCPURemainingTime() < S->Get_RTF()) { //Migrate Multiple Processes in the same time step until a process have Remaining Time less than RTF

			S->RRMigration(R);

			if (RDY_LIST.dequeue(R)) {
				State = BUSY;
				if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
				R->SetState(RUn);
				R->SetProcessor(this);
				QFT -= R->GetCPURemainingTime();
			}
			else { State = IDLE; break; }
		}
	}
}