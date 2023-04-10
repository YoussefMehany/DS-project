#include"Process Scheduler.h"
Scheduler::Scheduler()
{
	TimeStep = NS = NF = NR = Num_of_processors = RTF = M = MaxW = STL = Fork_Prob = 0;
	MultiProcessor = nullptr;
	srand(time(0));
}
void Scheduler::Get_Data()
{

}
Processor* Scheduler::Find_Shortest( int l, int r)
{
	Processor* Min = MultiProcessor[0];
	for (int i = l; i <= r; i++)
	{
		if (MultiProcessor[i]->GET_Total_CT() < Min->GET_Total_CT())
			Min = MultiProcessor[i];
	}
	return Min;
}
bool Scheduler::Processing()
{
	TimeStep++;
	if (TRM.GetSize() == M)
		return false;
	Process* temp = nullptr;
	while (!NEW.isEmpty()) //Check New List for Arriving Processes
	{
		NEW.peek(temp);
		if (temp->GetArrivalTime() == TimeStep)
		{
			NEW.dequeue(temp);
			To_RDY(temp);
		}
		else
			break;
	}
	int Rand = 0;
	Process* Curr = nullptr;
	for (int i = 0; i < Num_of_processors; i++) //Move process from RDY to RUN if processor is IDLE and Make the probability checking
	{
		Curr = MultiProcessor[i]->ScheduleAlgo();
		Rand = 1 + rand() % 100;
		if (Rand <= 15)
			TO_BLK(Curr);
		else if (Rand <= 30)
			To_RDY(Curr);
		else if (Rand <= 60)
			TO_TRM(Curr);
	}
	Rand = 1 + rand() % 100;
	if (Rand < 10) {
		BLK.dequeue(Curr);
		To_RDY(Curr);
	}
}
void Scheduler::To_RDY(Process* P)
{
	Processor* shortest = nullptr;
	P->SetState(RDY);
	shortest = Find_Shortest(0, Num_of_processors - 1);
	shortest->AddProcess(P);
}
void Scheduler::TO_BLK(Process* P)
{
	P->SetState(BLk);
	BLK.enqueue(P);
}
void Scheduler::TO_TRM(Process* P)
{
	P->SetState(TRm);
	TRM.enqueue(P);
}
// those cout will be removed and UI class will be used instead also override << in processor and process class to print the ID 
void Scheduler::PrintData() const
{
	cout << "Current Timestep:" << TimeStep << endl;
	cout << "--------------		RDY processes	--------------" << endl;
	for (int i = 0; i < Num_of_processors - 1; i++)
		MultiProcessor[i]->Print();
	cout << "--------------		BLK processes	--------------" << endl;
	BLK.Print();
	cout << "--------------		RUN processes	--------------" << endl;
	for (int i = 0; i < Num_of_processors - 1; i++)
		MultiProcessor[i]->Print();
	cout << "--------------		TRM processes	--------------" << endl;
	TRM.Print();
	cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!" << endl;
}