#include "SJF.h"
#include "../Process Scheduler/Process Scheduler.h"


SJF::SJF(Scheduler* Sched)
	:Processor(Sched) {}
void SJF::ScheduleAlgo() {
	Process* process = nullptr;
	if (State == IDLE && RDY_LIST.dequeue(process)) {
		State = BUSY;
		R = process;
		R->SetState(RUn);
		process->SetProcessor(this);
	}
}
void SJF::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.enqueue(process, process->GetCPUTime());
}
void SJF::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[SJF]: " + to_string(RDY_LIST.getSize()) + " RDY: ");
	RDY_LIST.Print();
}
void SJF::Kill(int PID) {}
int  SJF::GET_QFT()const {
	return QFT;
}