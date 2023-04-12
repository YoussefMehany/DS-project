#include "SJF.h"
#include "../Process Scheduler/Process Scheduler.h"


SJF::SJF(Scheduler* Sched)
	:Processor(Sched) {}
Process* SJF::ScheduleAlgo() {
	Process* process = nullptr;
	if (State == IDLE && RDY_LIST.dequeue(process)) {
		State = BUSY;
		R = process;
		process->SetProcessor(this);
	}
	return process;
}
void SJF::AddProcess(Process* process) {
	UpdateState(process);
	RDY_LIST.enqueue(process, process->GetCPUTime());
	Total_CT += process->GetCPUTime();
}
int SJF::GET_Total_CT()const{
	return Total_CT;
}
void SJF::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[SJF]: " + to_string(RDY_LIST.getSize()) + " RDY: ");
	RDY_LIST.Print();
}