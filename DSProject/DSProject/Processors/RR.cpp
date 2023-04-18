#include "RR.h"
#include "../Process Scheduler/Process Scheduler.h"


RR::RR(Scheduler* Sched, int tsr)
	:Processor(Sched), TSR(tsr) {}
void RR::ScheduleAlgo() {
	Process* process = nullptr;
	if (State == IDLE && RDY_LIST.dequeue(process)) {
		State = BUSY;
		R = process;
		R->SetState(RUn);
		process->SetProcessor(this);
	}
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
void RR::Kill(int PID) {}
int  RR::GET_QFT()const {
	return QFT;
}