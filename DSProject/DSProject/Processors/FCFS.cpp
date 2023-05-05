#include "FCFS.h"
#include "../Process Scheduler/Process Scheduler.h"

FCFS::FCFS(Scheduler* Sched)
	:Processor(Sched) {}


void FCFS::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.RemoveHead(R)) {

		R->AddWaitingTime(S->Get_TimeStep() - R->GetLastRunTime());  // This Line and the next one should be added to all processors  
		R->SetLastRunTime(S->Get_TimeStep());

		State = BUSY;
		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
		R->SetState(RUn);
		R->SetProcessor(this);

		QFT -= R->GetCPUTime();

		FCFSMigration(); //check for Migration Process First 
	}
	if (State == BUSY) {

		TBT++;
		R->UpdateInfo();

		if (!R->GetCPUTime()) {
			S->TO_TRM(R);
			R->SetChildState(); //set children to orphans
		}
		else if (R->GetIO() && !R->GetIO()->getFirst())
			S->TO_BLK(R);

	}
	else TIT++;
}

void FCFS::RemoveOrphans() {
	for (int i = 0; i < RDY_LIST.size(); i++) {
		Process* p = nullptr;
		RDY_LIST.GetItem(i, p);
		if (p->GetState() == ORPH) {
			S->TO_TRM(p);
			RDY_LIST.Remove(i, p);
			//QFT -= p->GetCPUTime(); need to discuss
		}
	}
}

void FCFS::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.InsertEnd(process);
	QFT += process->GetCPUTime();
}
void FCFS::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[FCFS]: " + to_string(RDY_LIST.size()) + " RDY: ");
	RDY_LIST.print();
}
void FCFS::Kill(int PID) {
	Process* p = nullptr;
	if (State == BUSY && R->GetPID() == PID) {
		S->TO_TRM(R);
		return;
	}
	for (int i = 0; i < RDY_LIST.size(); i++) {
		RDY_LIST.GetItem(i, p);
		if (p->GetPID() == PID) {
			RDY_LIST.Remove(i, p);
			QFT -= p->GetCPUTime();
			S->TO_TRM(p);
			return;
		}
	}
}

void FCFS::FCFSMigration() {

	if (S->Get_NR() && !R->GetParent()) { //Don't enter if no RR exists or process is a child because children must be in fcfs only
		while (R->GetCurrWaitingTime() > S->Get_MaxW()) { //Migrate Multiple Processes in the same time step until a process have Waiting Time less than MaxW

			S->FCFSMigration(R);

			if (RDY_LIST.RemoveHead(R)) {
				State = BUSY;
				if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
				R->SetState(RUn);
				R->SetProcessor(this);
				QFT -= R->GetCPUTime();
			}
			else { State = IDLE; break; }
		}
	}
}

void FCFS::Lose(Process*& Stolen) {

	if ( ( RDY_LIST.head()->GetParent() && !dynamic_cast<FCFS*>(S->GetSQ()) ) || !RDY_LIST.RemoveHead(Stolen)) Stolen = nullptr;
	else QFT -= Stolen->GetCPUTime();

}
