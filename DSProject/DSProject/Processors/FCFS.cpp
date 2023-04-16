#include "FCFS.h"
#include "../Process Scheduler/Process Scheduler.h"

FCFS::FCFS(Scheduler* Sched)
	:Processor(Sched) {}

void FCFS::ScheduleAlgo() {
	Process* process = nullptr;
	if (State == IDLE && RDY_LIST.RemoveHead(process)) {
		State = BUSY;
		R = process;
		R->SetState(RUn);
		process->SetProcessor(this);
	}
}
void FCFS::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.InsertEnd(process);
}
int FCFS::GET_QFT()const {
	return QFT;
}
void FCFS::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[FCFS]: " + to_string(RDY_LIST.size()) + " RDY: ");
	RDY_LIST.print();
}
void FCFS::Kill(int PID) {
	Process* p = nullptr;
	int i = 0;
	for (; i < RDY_LIST.size(); i++) {
		RDY_LIST.GetItem(i, p);
		if (p->GetPID() == PID) {
			RDY_LIST.Remove(i, p);
			S->TO_TRM(p);
			return;
		}
	}
}