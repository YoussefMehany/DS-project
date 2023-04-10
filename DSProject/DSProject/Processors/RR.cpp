#include "RR.h"

RR::RR()
	:Processor() {}
Process* RR::ScheduleAlgo() {
	if (!RDY_LIST.isEmpty() && State == IDLE) {
		Process* Head = nullptr;
		RDY_LIST.dequeue(Head);
		return Head;
	}
	return nullptr;
}
void RR::AddProcess(Process* process) {
	RDY_LIST.enqueue(process);
	Total_CT += process->GetCPUTime();
}
int RR::GET_Total_CT()const {
	return Total_CT;
}
void RR::Print() {}