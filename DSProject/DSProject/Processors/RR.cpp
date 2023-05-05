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
	if (State == BUSY) {
		TBT++;
	}
	else {
		TIT++;
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
void RR::Lose(Process*& Stolen) {
	if (!RDY_LIST.dequeue(Stolen)) Stolen = nullptr;
	else QFT -= Stolen->GetCPUTime();
}