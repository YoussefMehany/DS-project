#include "FCFS.h"
#include "../Process Scheduler/Process Scheduler.h"

FCFS::FCFS(Scheduler* Sched, int n)
	:Processor(Sched, n) {}


void FCFS::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.RemoveHead(R)) {


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
	else if (State == STOP) {
		if (N_TEMP == N) {
			if (R) {
				R->SetProcessor(nullptr);
				S->TO_SHORTEST_RDY(R, true);
			}
			int Size = RDY_LIST.size();
			while (Size--) {
				RDY_LIST.RemoveHead(R);
				R->SetProcessor(nullptr);
				S->TO_SHORTEST_RDY(R, true);
			}
			R = nullptr;
			QFT = 0;

		}
		else if (!N_TEMP) {
			State = IDLE;
			N_TEMP = N;
			return;
		}
		if (State == STOP) N_TEMP--;
		else N_TEMP = N;
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
			i--;
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
	Colors color = State == STOP ? RED : State == BUSY ? GREEN : WHITE;
	pOut->PrintOut("Processor " + to_string(ID), color);
	pOut->PrintOut("[FCFS]: " + to_string(RDY_LIST.size()) + " RDY: ", color);
	pOut->PrintColor(color);
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
			QFT -= p->GetCPURemainingTime();
			p->SetChildrenState();
			S->TO_TRM(p);
			return;
		}
	}
}

void FCFS::FCFSMigration() {

	if (S->Get_NR()) { //Don't enter if no RR exists or process is a child because children must be in fcfs only
		S->DecideShortest(2);
		if (!S->GetSRR()) return;
		while (R && R->GetCurrWaitingTime(S->Get_TimeStep()) > S->Get_MaxW() && !R->GetParent() && R->GetState() != ORPH) {
			S->FCFSMigration(R);
			while (RDY_LIST.RemoveHead(R)) {
				if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());
				R->SetState(RUn);
				State = BUSY;
				QFT -= R->GetCPURemainingTime();
				break;
			}
		}
	}
}

void FCFS::Forking() {
	int FP = S->Get_FP();
	int Rand = 1 + rand() % 100;
	if (R && Rand <= FP && (!R->GetLeftChild() || !R->GetRightChild())) {
		Process* child = S->AddChildToSQ(R->GetCPURemainingTime(),R->GetDeadLine());
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
		if (!p->GetParent() && p->GetState() != ORPH) {
			RDY_LIST.Remove(i, p);
			QFT -= p->GetCPURemainingTime();
			p->SetProcessor(nullptr);
			Stolen = p;
			break;
		}
	}
}
