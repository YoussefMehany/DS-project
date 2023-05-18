#include "SJF.h"
#include "../Process Scheduler/Process Scheduler.h"


SJF::SJF(Scheduler* Sched, int n)
	:Processor(Sched, n) {}
void SJF::ScheduleAlgo() {

	if (State == IDLE && RDY_LIST.dequeue(R)) {

		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep()); //Response time setting
		AddToRun();
	}
	if (State == BUSY) {
		RunProcess();
	}
	else if (State == STOP) { //Overheat state
		OverHeat();
	}
	else TIT++;

}
void SJF::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);
	RDY_LIST.enqueue(process, process->GetCPURemainingTime());
	QFT += process->GetCPURemainingTime();
}
void SJF::RunProcess() {
	TBT++;
	R->UpdateInfo();
	if (!R->GetCPURemainingTime()) //Move to TRM if the CPU remaining time is zero
		S->TO_TRM(R);
	else if (R->GetIO() && !R->GetIO()->getFirst()) //Move to BLK if the IO duration ended
		S->TO_BLK(R);
}

void SJF::OverHeat() {
	if (N_TEMP == N) {
		if (R) {
			R->SetProcessor(nullptr);
			S->TO_SHORTEST_RDY(R);
		}

		int Size = RDY_LIST.getSize();

		while (Size--) {
			RDY_LIST.dequeue(R);
			R->SetProcessor(nullptr);
			S->TO_SHORTEST_RDY(R);
		}

		QFT = 0;
		R = nullptr;
	}
	else if (!N_TEMP) { //If Cooldown ended then reset heat factor and return back to idle 
		State = IDLE;
		N_TEMP = N;
		return;
	}
	if (State == STOP) N_TEMP--;
	else N_TEMP = N;
}

void SJF::Print() {
	Output* pOut = S->getOutput();
	Colors color = State == STOP ? RED : State == BUSY ? GREEN : WHITE;
	pOut->PrintOut("Processor " + to_string(ID), color);
	pOut->PrintOut("[SJF]: " + to_string(RDY_LIST.getSize()) + " RDY: ", color);
	pOut->PrintColor(color);
	RDY_LIST.Print();
}
void SJF::Lose(Process*& Stolen) { // Work stealing function
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}
