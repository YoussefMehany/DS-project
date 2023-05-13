#include "Processor.h"

int Processor::Counter = 0;

Processor::Processor(Scheduler* Sched, int n)
	:S(Sched), QFT(0), State(IDLE), ID(++Counter), R(nullptr), TIT(0), TBT(0), pLoad(0), pUtil(0), N_TEMP(n), N(n) {}

void Processor::UpdateState() {
	if (!R || R->GetState() != RUn) {
		State = IDLE;
		R = nullptr;
	}
}

void Processor::SetProcessorState(ProcessorState processor_state) {
	State = processor_state;
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
int Processor::Get_HeatFactor()const {
	return N_TEMP;
}

void Processor::Reset_HeatFactor() {
	N_TEMP = N;
}

double Processor::Get_pUtil() const {
	return (double(TBT) / (TIT + TBT)) * 100;
}
double Processor::Get_pLoad() const {
	return (double(TBT) / Process::GetTTAT()) * 100;
}