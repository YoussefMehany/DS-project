#include "Processor.h"

int Processor::Counter = 0;
Processor::Processor(Scheduler* Sched)
	:S(Sched), QFT(0), State(IDLE), ID(++Counter), R(nullptr), TIT(0), TBT(0), pLoad(0), pUtil(0) {}

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
int Processor::GET_QFT()const{
	return QFT;
}
double Processor::Get_pUtil() const {
	return (double(TBT) / (TIT + TBT)) * 100;
}