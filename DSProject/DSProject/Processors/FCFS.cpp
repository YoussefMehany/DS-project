#include "FCFS.h"

FCFS::FCFS()
	:Processor() {}
Process* FCFS::ScheduleAlgo() {
	Scheduler S;
    Process* process=nullptr;
	int Rand = 1 + rand() % RDY_LIST.size();
	RDY_LIST.Remove(Rand, process);
	S.TO_TRM(process); //Move Random process to TRM List
	if (RDY_LIST.RemoveHead(process) && State == IDLE)
		return process;
	return nullptr;
}
void FCFS::AddProcess(Process* process) {
	RDY_LIST.InsertEnd(process);
	Total_CT += process->GetCPUTime();
}
int FCFS::GET_Total_CT()const {
	return Total_CT;
}
void FCFS::Print() {}