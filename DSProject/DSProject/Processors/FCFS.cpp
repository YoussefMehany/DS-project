#include "FCFS.h"
#include "../Process Scheduler/Process Scheduler.h"

FCFS::FCFS(Scheduler* Sched)
	:Processor(Sched) {}


void FCFS::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.RemoveHead(R)) {

		R->AddWaitingTime(S->Get_TimeStep() - R->GetLastRunTime());  // This Line and the next one should be added to all processors  

		State = BUSY;
		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
		R->SetState(RUn);
		
		QFT -= R->GetCPURemainingTime();

		FCFSMigration(); //check for Migration Process First 
	}
	if (State == BUSY) {

		TBT++;
		R->UpdateInfo();

		if (!R->GetCPURemainingTime()) {
			R->SetChildrenState(); //set children to orphans
			S->TO_TRM(R);
		}
		else if (R->GetIO() && !R->GetIO()->getFirst())
			S->TO_BLK(R);

	}
	else TIT++;
}

void FCFS::RemoveOrphans() {

	if (R && R->GetState() == ORPH) {

		QFT -= R->GetCPURemainingTime();

		S->TO_TRM(R);
	}
	for (int i = 0; i < RDY_LIST.size(); i++) {

		Process* p = nullptr;

		RDY_LIST.GetItem(i, p);

		if (p->GetState() == ORPH) {

			QFT -= p->GetCPURemainingTime(); //need to discuss

			RDY_LIST.Remove(i, p);

			S->TO_TRM(p);

		}
	}
}

void FCFS::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.InsertEnd(process);
	QFT += process->GetCPURemainingTime();
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
		R->SetChildrenState();
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
	/// <question>
	/// shouldn't we check if its in the fork tree and not just that it has no parent.. ?
	/// </question>
	if (S->Get_NR() && !R->GetParent()) { //Don't enter if no RR exists or process is a child because children must be in fcfs only
		while (R->GetCurrWaitingTime() > S->Get_MaxW()) { //Migrate Multiple Processes in the same time step until a process have Waiting Time less than MaxW

			S->FCFSMigration(R);

			if (RDY_LIST.RemoveHead(R)) {
				State = BUSY;
				if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
				R->SetState(RUn);
				QFT -= R->GetCPUTime();
			}
			else { State = IDLE; break; }
		}
	}
}

void FCFS::Forking() {
	int FP = S->Get_FP();
	int Rand = 1 + rand() % 100;
	if (R && Rand <= FP && (!R->GetLeftChild() && !R->GetRightChild())) {
		Process* child = S->AddChildToSQ(R->GetArrivalTime(), R->GetCPURemainingTime());
		if (!R->GetLeftChild()) {
			R->SetLeftChild(child);
		}
		else if (!R->GetRightChild()) {
			R->SetRightChild(child);
		}
	}
}

void FCFS::Lose(Process*& Stolen) {
	Process* p = nullptr;

	for (int i = 0; i < RDY_LIST.size(); i++) {

		RDY_LIST.GetItem(i, p);

		if (!p->GetParent()) {

			RDY_LIST.Remove(i, p);

			QFT -= p->GetCPURemainingTime();

			Stolen = p;

		}
	}

}
