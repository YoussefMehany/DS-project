#include "Process.h"

int Process::counter = 1;

Process::Process(int ArrivalTime, int CPUTime, int IORequestTime, int IODuration) {
	this->ArrivalTime = ArrivalTime;
	this->CPUTime = CPUTime;
	this->IORequestTime = IORequestTime;
	this->IODuration = IODuration;
	TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = 0;
	PID = counter++;
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