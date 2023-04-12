#include "Output.h"

Output::Output() {}


void Output::PrintInfo(Processor** Multiprocessors, int size, Queue<Process*> BLK, Queue<Process*> TRM, Queue<Process*> RUN, int timestep)
{
	cout << "Current Timestep: " << timestep << endl;
	cout << "--------------		RDY processes	--------------" << endl;
	for (int i = 0; i < size; i++) {
		Multiprocessors[i]->Print();
	}
	cout << "--------------		BLK processes	--------------" << endl;
	cout << BLK.GetSize() << " BLK: ";
	BLK.Print();
	cout << "--------------		RUN processes	--------------" << endl;
	cout << RUN.GetSize() << " RUN: ";
	RUN.Print();
	cout << "--------------		TRM processes	--------------" << endl;
	cout << TRM.GetSize() << " TRM: ";
	TRM.Print();
	cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!" << endl;
}
void Output::PrintOut(string word)
{
	cout << word;
}