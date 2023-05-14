#include "Output.h"

Output::Output() {}

void Output::PrintInfo(Processor** Multiprocessors, int size, Queue<Process*> BLK, Queue<Process*> TRM, int timestep)
{
	int Runcnt = 0;
	string Runlist;
	PrintColor(BOLDMAG); //Base Color
	PrintColor(WHITE);
	cout << "Current Timestep: " << timestep << endl << endl;
	PrintColor(CYAN);
	cout << "--------------   RDY processes   --------------" << endl;
	for (int i = 0; i < size; i++) {
		Multiprocessors[i]->Print();
	}
	PrintColor(CYAN);
	cout << "--------------   BLK processes   --------------" << endl;
	PrintColor(BLUE);
	cout << BLK.GetSize() << " BLK: ";
	BLK.Print();
	PrintColor(CYAN);
	cout << "--------------   RUN processes   --------------" << endl;
	PrintColor(YELLOW);
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
	cout << Runcnt << " RUN: " << Runlist << endl;
	PrintColor(CYAN);
	cout << "--------------   TRM processes   --------------" << endl;
	PrintColor(ORANGE);
	cout << TRM.GetSize() << " TRM: ";
	TRM.Print();
	cout << endl;
	PrintColor(WHITE);
}
void Output::PrintColor(Color clr) {
	switch (clr) {
		case BOLDMAG:
			cout << "\033[1m\033[35m";
			break;
		case RED:
			cout << "\033[91m";
			break;
		case MAGENTA:
			cout << "\033[35m";
			break;
		case YELLOW:
			cout << "\033[33m";
			break;
		case ORANGE:
			cout << "\033[38;2;255;165;0m";
			break;
		case WHITE:
			cout << "\033[37m";
			break;
		case GREEN:
			cout << "\033[32m";
			break;
		case CYAN:
			cout << "\033[36m";
			break;
		case BLUE:
			cout << "\033[34m";
			break;

	}
}
void Output::ClearConsole() {
	system("CLS");
}
void Output::PrintOut(string word) {
	cout << word;
}