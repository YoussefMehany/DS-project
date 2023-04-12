#include "RR.h"
#include "../Process Scheduler/Process Scheduler.h"


RR::RR(Scheduler* Sched)
	:Processor(Sched) {}

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
	UpdateState(process);
	RDY_LIST.enqueue(process);
	Total_CT += process->GetCPUTime();
}
int RR::GET_Total_CT()const {
	return Total_CT;
}
void RR::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[RR]: " + to_string(RDY_LIST.GetSize()) + " RDY: ");
	RDY_LIST.Print();
}