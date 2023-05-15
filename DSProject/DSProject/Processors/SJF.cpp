#include "SJF.h"
#include "../Process Scheduler/Process Scheduler.h"


SJF::SJF(Scheduler* Sched, int n)
	:Processor(Sched, n) {}
void SJF::ScheduleAlgo() {

	if (State == IDLE && RDY_LIST.dequeue(R)) {

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
	else if (State == STOP) {
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
		else if (!N_TEMP) {
			State = IDLE;
			N_TEMP = N;
			return;
		}
		if (State == STOP) N_TEMP--;
		else N_TEMP = N;
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
	Colors color = State == STOP ? RED : State == BUSY ? GREEN : WHITE;
	pOut->PrintOut("Processor " + to_string(ID), color);
	pOut->PrintOut("[SJF]: " + to_string(RDY_LIST.getSize()) + " RDY: ", color);
	pOut->PrintColor(color);
	RDY_LIST.Print();
}
void SJF::Lose(Process*& Stolen) {
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}
