#include "SJF.h"
#include "../Process Scheduler/Process Scheduler.h"


SJF::SJF(Scheduler* Sched)
	:Processor(Sched) {}
void SJF::ScheduleAlgo() {
	
	if (State == IDLE && RDY_LIST.dequeue(R)) {
		R->AddWaitingTime(S->Get_TimeStep() - R->GetLastRunTime());

        if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep());

		State = BUSY;
	
		R->SetState(RUn);
		QFT -= R->GetCPURemainingTime();
	}
	if (State == BUSY) {
		TBT++;
		R->UpdateInfo();
		if (!R->GetCPURemainingTime()) 
			S->TO_TRM(R);
		
		else if (R->GetIO() && !R->GetIO()->getFirst())
			S->TO_BLK(R);
	}
	else TIT++;

}
void SJF::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.enqueue(process, process->GetCPURemainingTime());
	QFT += process->GetCPURemainingTime();
}
void SJF::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[SJF]: " + to_string(RDY_LIST.getSize()) + " RDY: ");
	RDY_LIST.Print();
}
void SJF::Lose(Process*& Stolen) {
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}