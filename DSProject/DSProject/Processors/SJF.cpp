#include "SJF.h"

SJF::SJF()
	:Processor() {}
Process* SJF::ScheduleAlgo() {
	Process* process;
	if (RDY_LIST.dequeue(process) && State == IDLE)
		return process;
	return nullptr;
}
void SJF::AddProcess(Process* process) {
	RDY_LIST.enqueue(process, process->GetCPUTime());
	Total_CT += process->GetCPUTime();
}
int SJF::GET_Total_CT()const{
	return Total_CT;
}
void SJF::Print() {}