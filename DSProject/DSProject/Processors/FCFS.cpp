#include "FCFS.h"
#include "../Process Scheduler/Process Scheduler.h"

FCFS::FCFS(Scheduler* Sched)
	:Processor(Sched) {}

Process* FCFS::ScheduleAlgo() {
	Process* process = nullptr;
	int Rand = 0;
	if (!RDY_LIST.Empty())
		Rand = rand() % RDY_LIST.size();
	else return nullptr;
	if (RDY_LIST.Remove(Rand, process)) {
		S->TO_TRM(process); //Move Random process to TRM List
	}
	process = nullptr;
	if (State == IDLE && RDY_LIST.RemoveHead(process)) {
		State = BUSY;
		R = process;
		process->SetProcessor(this);
	}
	return process;
}
void FCFS::AddProcess(Process* process) {
	UpdateState();
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