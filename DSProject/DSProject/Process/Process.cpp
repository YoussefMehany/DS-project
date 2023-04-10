#include "Process.h"

int Process::counter = 1;

Process::Process() {}
Process::Process(int ArrivalTime, int CPUTime, int IORequestTime, int IODuration) {
	this->ArrivalTime = ArrivalTime;
	this->CPUTime = CPUTime;
	this->IORequestTime = IORequestTime;
	this->IODuration = IODuration;
	TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = 0;
	PID = counter++;
	State = NEW;
}
int Process::GetPID()const {
	return PID;
}
int Process::GetArrivalTime()const {
	return ArrivalTime;
}
int Process::GetCPUTime()const {
	return CPUTime;
}
int Process::GetTerminationTime()const {
	return TerminationTime;
}
int Process::GetResponseTime()const {
	return ResponseTime;
}
int Process::GetTurnAroundDuration()const {
	return TurnAroundDuration;
}
int Process::GetWaitingTime()const {
	return WaitingTime;
}
int Process::GetIORequestTime()const {
	return IORequestTime;
}
int Process::GetIODuration()const {
	return IODuration;
}
ProcessState Process::GetState()const {
	return State;
}
void Process::SetTerminationTime(int TerminationTime) {
	this->TerminationTime = TerminationTime;
}
void Process::SetResponseTime(int TimeFirst) {
	ResponseTime = TimeFirst - ArrivalTime;
}
void Process::SetTurnAroundDuration() {
	TurnAroundDuration = TerminationTime - ArrivalTime;
}
void Process::SetWaitingTime() {
	WaitingTime = TurnAroundDuration - CPUTime;
}
void Process::SetState(ProcessState state) {
	State = state;
}
