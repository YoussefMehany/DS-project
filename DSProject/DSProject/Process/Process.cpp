#include "Process.h"
#include <fstream>
int Process::TTAT = 0;
int Process::LastID = 0;
Process::Process() {
	PID = CPUTime = ArrivalTime = TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime =  0;
	State = NEW;
	Parent = Lchild = Rchild = nullptr;
	RunProcessor = nullptr;
}
Process::Process(int ArrivalTime, int CPUTime,int Deadline ,int PID) {
	this->ArrivalTime = ArrivalTime;
	this->CPUTime = CPUTime;
	CPUTemp = CPUTime;
	Ex_DeadLine= Deadline;
	TIOD = TerminationTime = TurnAroundDuration = WaitingTime = ResponseTime = 0;
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
int Process::GetCurrWaitingTime(int TimeStep)const { 
	return TimeStep - ArrivalTime - (CPUTime - CPUTemp);
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
int Process::GetTIOD()const{
	return TIOD;
}
int Process::GetDeadLine()const {
	return Ex_DeadLine;
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
	TurnAroundDuration = TerminationTime - ArrivalTime + 1; // + 1 Because it runs one time in the same timestep that it terminates in, so we should add that too
	TTAT += TurnAroundDuration;
}
int Process::GetTTAT() {
	return TTAT;
}
void Process::SetWaitingTime() { //Added that CPUTime - CPUTemp means to subtract the execution time only and not the whole cpu time
	 WaitingTime =  TurnAroundDuration - (CPUTime-CPUTemp); //To avoid negative waiting time when a process is killed or terminated before its finish time
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
void Process::SetTIOD(int TIOD) { //Total IO duration
	this->TIOD = TIOD;
}
void Process::UpdateInfo() {
	if (CPUTemp) CPUTemp--;

	Pair<int, int>* IO = nullptr;
	IO_LIST.peek(IO);

	if (IO)
		if (!IO->getFirst())      ///If we entered and found out that the IO_R is 0 this means that this process is scheduled by EDF so it wont make an IO so we 
			IO_LIST.dequeue(IO); ///Removed it here so that when it goes to another one it make the remaining IOs 
		else 
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