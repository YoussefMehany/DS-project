#include "Output.h"

Output::Output() { }

void Output::PrintInfo(Processor** Multiprocessors, int size, Queue<Process*> BLK, Queue<Process*> TRM, int timestep)
{
	int Runcnt = 0;
	string Runlist;
	cout << "Current Timestep: " << timestep << endl;
	cout << "--------------		RDY processes	--------------" << endl;
	for (int i = 0; i < size; i++) {
		Multiprocessors[i]->Print();
	}
	cout << "--------------		BLK processes	--------------" << endl;
	cout << BLK.GetSize() << " BLK: ";
	BLK.Print();
	cout << "--------------		RUN processes	--------------" << endl;
	for (int i = 0; i < size; i++) {
		if (Multiprocessors[i]->Get_State() == BUSY) {
			Runcnt++;
			Runlist += to_string(Multiprocessors[i]->Get_Run()->GetPID()) + ", ";
		}
	}
	if (!Runlist.empty()) {
		Runlist.pop_back();
		Runlist.pop_back();
	}
	cout << Runcnt << " RUN: " << Runlist << endl;;
	cout << "--------------		TRM processes	--------------" << endl;
	cout << TRM.GetSize() << " TRM: ";
	TRM.Print();
}
void Output::PrintOut(string word){
	cout << word;
}