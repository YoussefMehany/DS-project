#include "SJF.h"

SJF::SJF()
	:Processor() {}
Process* SJF::ScheduleAlgo() {
	Process process;
	if (RDY_LIST.dequeue(process) && State == IDLE)
		return &process;
	return nullptr;
}
void SJF::AddProcess(Process* process) {
	RDY_LIST.enqueue(*process, process->GetCPUTime());
}