#include "Process.h"


             Process::Process() {
	PID = CPUTime = ArrivalTime = TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = 0;
	State = NEW;
	Child = nullptr;
	RunProcessor = nullptr;
}
             Process::Process(int ArrivalTime, int PID, int CPUTime) {
	this->ArrivalTime = ArrivalTime;
	this->CPUTime = CPUTime;
	TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = 0;
	this->PID = PID;
	State = NEW;
	Child = nullptr;
	RunProcessor = nullptr;
}
int          Process::GetPID()const {
	return PID;
}
int          Process::GetArrivalTime()const {
	return ArrivalTime;
}
int          Process::GetCPUTime()const {
	return CPUTime;
}
int          Process::GetTerminationTime()const {
	return TerminationTime;
}
int          Process::GetResponseTime()const {
	return ResponseTime;
}
int          Process::GetTurnAroundDuration()const {
	return TurnAroundDuration;
}
int          Process::GetWaitingTime()const {
	return WaitingTime;
}
Pair<int>&   Process::GetIO() {
	Pair<int>* IO;
	IO_LIST.dequeue(IO);
	return *IO;
}
Process*     Process::GetChild()const {
	return Child;
}
ProcessState Process::GetState()const {
	return State;
}
void         Process::AddIO(int IO_R, int IO_D) {
	Pair<int>* IO = new Pair<int>(IO_R, IO_D);
	IO_LIST.enqueue(IO);
}
void         Process::SetTerminationTime(int TerminationTime) {
	this->TerminationTime = TerminationTime;
}
void		 Process::SetResponseTime(int TimeFirst) {
	ResponseTime = TimeFirst - ArrivalTime;
}
void         Process::SetTurnAroundDuration() {
	TurnAroundDuration = TerminationTime - ArrivalTime;
}
void         Process::SetWaitingTime() {
	WaitingTime = TurnAroundDuration - CPUTime;
}
void         Process::SetState(ProcessState state) {
	State = state;
}
void         Process::SetProcessor(Processor* processor) {
	RunProcessor = processor;
}
void         Process::SetChild(Process* child) {
	Child = child;
}
Processor*   Process::GetProcessor() const {
	return RunProcessor;
}
ostream& operator<<(ostream& out, const Process& process)
{
	out << process.PID;
	return out;
}