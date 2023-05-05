#include "Process.h"

int Process::TTAT = 0;
Process::Process() {
	PID = CPUTime = ArrivalTime = TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = LastRunTime = CurrWaitingTime = 0;
	State = NEW;
	Parent = Child = nullptr;
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
int Process::GetCurrWaitingTime()const {
	return CurrWaitingTime;
}
int Process::GetLastRunTime()const {
	return LastRunTime;
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
Pair<int, int>* Process::GetIO() {
	Pair<int, int>* IO = nullptr;
	IO_LIST.peek(IO);
	return IO;
}
Process* Process::GetChild()const {
	return Child;
}
Process* Process::GetParent()const {
	return Parent;
}
ProcessState Process::GetState()const {
	return State;
}
void Process::AddIO(int IO_R, int IO_D) {
	Pair<int, int>* IO = new Pair<int, int>(IO_R, IO_D);
	IO_LIST.enqueue(IO);
}
void Process::SetTerminationTime(int TerminationTime) {
	this->TerminationTime = TerminationTime;
}
void Process::SetResponseTime(int TimeFirst) {
	ResponseTime = TimeFirst - ArrivalTime;
}
void Process::SetTurnAroundDuration() {
	TurnAroundDuration = TerminationTime - ArrivalTime;
	TTAT += TurnAroundDuration;
}
int Process::GetTTAT() {
	return TTAT;
}
void Process::SetWaitingTime() {
	WaitingTime = TurnAroundDuration - CPUTime;
}
void Process::SetState(ProcessState state) {
	State = state;
}
void Process::SetProcessor(Processor* processor) {
	RunProcessor = processor;
}
void Process::SetChild(Process* child) {
	if(child) 
		child->SetParent(this);
	Child = child;
}
void Process::SetParent(Process* parent) {
	Parent = parent;
}
void Process::AddWaitingTime(int Time) {
	CurrWaitingTime += Time;
}
void Process::SetLastRunTime(int Time) {
	LastRunTime = Time;
}
void Process::UpdateInfo() {
	if (CPUTime) CPUTime--;

	Pair<int, int>* IO = nullptr;
	IO_LIST.peek(IO);
	if (IO && IO->getFirst())
		IO->SetFirst(IO->getFirst() - 1);
}
void Process::SetChildState(bool par) { //to set all children states to be orphans
	if (Parent) {
		Parent->SetChild(nullptr);
	}
	if (par) {
		State = TRm;
	}
	else State = ORPH;
	if (Child) {
		Child->SetChildState(false);
	}
}
Processor* Process::GetProcessor() const {
	return RunProcessor;
}
ostream& operator<<(ostream& out, const Process& process)
{
	out << process.PID;
	return out;
}