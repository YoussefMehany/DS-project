#include "RR.h"

RR::RR()
	:Processor() {}
Process* RR::ScheduleAlgo() {
	if (!RDY_LIST.Empty() && State == IDLE) {
		Process Head = RDY_LIST.HeadItem();
		RDY_LIST.RemoveBeg();
		return &Head;
	}
	return nullptr;
}
void RR::AddProcess(Process* process) {
	RDY_LIST.InsertEnd(*process);
}