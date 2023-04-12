#include "Processor.h"

int Processor::Counter = 0;
Processor::Processor(Scheduler* Sched)
	:S(Sched), Total_CT(0), State(IDLE), ID(++Counter), R(nullptr) {}

void Processor::UpdateState(Process* process) {
	if (process == R) {
		State = IDLE;
		R = nullptr;
	}
}