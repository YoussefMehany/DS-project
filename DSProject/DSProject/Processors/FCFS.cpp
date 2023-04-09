#include "FCFS.h"

FCFS::FCFS()
	:Processor() {}
Process* FCFS::ScheduleAlgo() {
	Process process;
	if (RDY_LIST.dequeue(process) && State == IDLE)
		return &process;
	return nullptr;
}
void FCFS::AddProcess(Process* process) {
	RDY_LIST.enqueue(*process);
}