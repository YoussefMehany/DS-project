#include "RR.h"
#include "../Process Scheduler/Process Scheduler.h"


RR::RR(Scheduler* Sched,int tsr)
	:Processor(Sched),TSR(tsr) {}

Process* RR::ScheduleAlgo() {
	Process* process = nullptr;
	if (State == IDLE && RDY_LIST.dequeue(process)) {
		State = BUSY;
		R = process;
		process->SetProcessor(this);
	}
	return process;
}
void RR::AddProcess(Process* process) {
	UpdateState();
	RDY_LIST.enqueue(process);
}
int RR::GET_QFT()const {
	return QFT;
}
void RR::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[RR]: " + to_string(RDY_LIST.GetSize()) + " RDY: ");
	RDY_LIST.Print();
}