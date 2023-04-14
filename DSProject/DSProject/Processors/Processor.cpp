#include "Processor.h"

int Processor::Counter = 0;
Processor::Processor(Scheduler* Sched)
	:S(Sched), QFT(0), State(IDLE), ID(++Counter), R(nullptr) {}

void Processor::UpdateState() {
	if (!R || R->GetState() != RUn) {
		State = IDLE;
		R = nullptr;
	}
}
ProcessorState Processor::Get_State()const {
	return State;
}
Process* Processor::Get_Run()const {
	return R;
}