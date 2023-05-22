#include "EDF.h"
#include "../Process Scheduler/Process Scheduler.h"

///As Explained in the document the processes in the EDF run until it finishes or a new process with an earlier deadline arrives
///so it wont go to the BLK for IO.
///The only chance that a process can go to BLK after entering the EDF is when the processor gets overheated so all 
///the processes will go to another processors and make the remaining IOs, or anthor processor Steals from it.

EDF::EDF(Scheduler* Sched, int n)
	:Processor(Sched, n) {}

void EDF::ScheduleAlgo() {
	if (State == IDLE && RDY_LIST.dequeue(R)) {
		if (!R->GetResponseTime()) R->SetResponseTime(S->Get_TimeStep()); //Response time setting
		AddToRun();
	}
	if (State == BUSY) {
		RunProcess();
	}
	else if (State == STOP) { //Overheat State
		OverHeat();
	}
	else TIT++;

}
void EDF::RunProcess() {
	TBT++;
	R->UpdateInfo();
	if (!R->GetCPURemainingTime())
		S->TO_TRM(R);
}
void EDF::OverHeat() {
	if (N_TEMP == N) {
		QFT = 0;
		int Size = RDY_LIST.getSize();
		if (R) {
			R->SetProcessor(nullptr);
			S->TO_SHORTEST_RDY(R);
		}
		while (Size--) {
			RDY_LIST.dequeue(R);
			R->SetProcessor(nullptr);
			S->TO_SHORTEST_RDY(R);
		}

		R = nullptr;
	}
	else if (!N_TEMP) { //when cooldown ends, reset heat factor and return back to idle state
		State = IDLE;
		N_TEMP = N;
		return;
	}
	if (State == STOP) N_TEMP--;
	else N_TEMP = N;
}

void EDF::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);

	if (State == BUSY && process->GetDeadLine() < R->GetDeadLine()) { //swap the processes in Run with the process in rdy list if it has an earlier deadline
		swap(process, R);
		R->SetState(RUn);
		process->SetState(RDy);
	}

	RDY_LIST.enqueue(process, process->GetDeadLine()); //enqueue in prio queue based on deadline
	QFT += process->GetCPURemainingTime();
}
void EDF::Print() {
	Output* pOut = S->getOutput();
	Colors color = State == STOP ? RED : State == BUSY ? GREEN : GREY;
	pOut->PrintOut("Processor " + to_string(ID), color);
	pOut->PrintOut("[EDF]: " + to_string(RDY_LIST.getSize()) + " RDY: ", color);
	pOut->PrintColor(color);
	RDY_LIST.Print();
}
void EDF::Lose(Process*& Stolen) {// Work stealing function
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}
