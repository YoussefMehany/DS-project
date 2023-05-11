#include "Process.h"
#include <fstream>
int Process::TTAT = 0;
int Process::LastID = 0;
Process::Process() {
	PID = CPUTime = ArrivalTime = TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime =  CurrWaitingTime = 0;
	LastRunTime = 1;
	State = NEW;
	Parent = Lchild = Rchild = nullptr;
	RunProcessor = nullptr;
}
Process::Process(int ArrivalTime, int CPUTime, int PID) {
	this->ArrivalTime = ArrivalTime;
	this->CPUTime = CPUTime;
	CPUTemp = CPUTime;
	TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = 0;
	if (~PID) {
		this->PID = PID;
		LastID = PID;
	}
	else {
		this->PID = ++LastID;
	}
	State = NEW;
	Lchild = Rchild = Parent = nullptr;
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
int Process::GetTIOD()const
{
	return TIOD;
}
int Process::GetCPURemainingTime()const {
	return CPUTemp;
}
Pair<int, int>* Process::GetIO() {
	Pair<int, int>* IO = nullptr;
	IO_LIST.peek(IO);
	return IO;
}
void Process::NextIO() {
	Pair<int, int>* IO = nullptr;
	IO_LIST.dequeue(IO);
}
Process* Process::GetLeftChild()const {
	return Lchild;
}
Process* Process::GetRightChild()const {
	return Rchild;
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
void Process::SetLeftChild(Process* child) {
	if (child) {
		child->Parent = this;
	}
	Lchild = child;
}
void Process::SetRightChild(Process* child) {
	if (child) {
		child->Parent = this;
	}
	Rchild = child;
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
void Process::SetTIOD(int TIOD) {
	this->TIOD = TIOD;
}
void Process::UpdateInfo() {
	if (CPUTemp) CPUTemp--;

	Pair<int, int>* IO = nullptr;
	IO_LIST.peek(IO);
	if (IO && IO->getFirst())
		IO->SetFirst(IO->getFirst() - 1);
}
void Process::SetChildrenState(bool par) { //to set all children states to be orphans
	if (Parent) {
		(Parent->Lchild && Parent->Lchild == this ? Parent->Lchild : Parent->Rchild) = nullptr;
	}
	if (par) {
		State = TRm;
	}
	else State = ORPH;
	if (Lchild) {
		Lchild->SetChildrenState(false);
	}
	if (Rchild) {
		Rchild->SetChildrenState(false);
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
void Process::PrintOutFile(ofstream& out)
{
	out << TerminationTime << '\t' << PID << '\t' << ArrivalTime << '\t' <<
		CPUTime << '\t'<<TIOD<<'\t'<<WaitingTime<<'\t'<<ResponseTime<<'\t'<<TurnAroundDuration
		<<'\n';
}