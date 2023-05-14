#include "EDF.h"
#include "../Process Scheduler/Process Scheduler.h"


EDF::EDF(Scheduler* Sched, int n)
	:Processor(Sched, n) {}
void EDF::ScheduleAlgo() {

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

		///As Explained in the document the processes in the EDF run until it finishes or a new process with an earlier deadline arrives
		///so it wont go to the BLK for IO.
		///The only chance that a process can go to BLK after entering the EDF is when the processor gets overheated so all 
		///the processes will go to another processors and make the remaining IOs, or anthor processor Steals from it.
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
void EDF::AddProcess(Process* process) {
	UpdateState();
	process->SetProcessor(this);

	if (State == BUSY && process->GetDeadLine() < R->GetDeadLine()) {
		swap(process, R);
		R->SetState(RUn);
		process->SetState(RDy);
	}

	RDY_LIST.enqueue(process, process->GetDeadLine());
	QFT += process->GetCPURemainingTime();
}
void EDF::Print() {
	Output* pOut = S->getOutput();
	pOut->PrintColor(State == STOP ? RED : State == BUSY ? GREEN : WHITE);
	pOut->PrintOut("Processor " + to_string(ID));
	pOut->PrintOut("[EDF]: " + to_string(RDY_LIST.getSize()) + " RDY: ");
	RDY_LIST.Print();
}
void EDF::Lose(Process*& Stolen) {
	RDY_LIST.dequeue(Stolen);
	QFT -= Stolen->GetCPURemainingTime();
}
