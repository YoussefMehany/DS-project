#include "Output.h"
#include <Windows.h>

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
			Runlist += to_string(Multiprocessors[i]->Get_Run()->GetPID()) + "(P"+to_string(i+1)+"), ";
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
void Output::PrintColor(Colors clr) { //Print the code that colors the terminal
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
void Output::PrintOut(string word, Colors color) {
	PrintColor(color);
	cout << word;
	PrintColor(WHITE);
}
void Output::LineBreaks(int count) {
	for (int i = 0; i < count; i++) {
		cout << '\n';
	}
}
void Output::PrintShow(string word, int duration) {
	for (int i = 0; i < word.size(); i++) {
		cout << word[i];
		Sleep(duration);
	}
}
void Output::PrintChar(char c, int count, Colors color) {
	PrintColor(color);
	for (int i = 0; i < count; i++) {
		cout << c;
	}
	PrintColor(WHITE);
}
void Output::Intro() { //Print the tabulated intro of the project
	PrintChar(' ', 10);
	PrintOut("+");
	PrintChar('-', 97, YELLOW);
	PrintOut("+");
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("|", YELLOW);
	PrintChar(' ', 40);
	PrintOut("Process Scheduler", CYAN);
	PrintChar(' ', 40);
	PrintOut("|", YELLOW);
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("|", YELLOW);
	PrintChar(' ', 27);
	PrintOut("https://github.com/YoussefMehany/DS-project");
	PrintChar(' ', 27);
	PrintOut("|", YELLOW);
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("+");
	PrintChar('-', 97, YELLOW);
	PrintOut("+");
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("|", YELLOW);
	PrintChar(' ', 1);
	PrintOut("This is the project of CMP1040 (Data Structures and Algorithms) course , The contributors are:", RED);
	PrintChar(' ', 2);
	PrintOut("|", YELLOW);
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("|", YELLOW);
	PrintChar(' ', 97);
	PrintOut("|", YELLOW);
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("|", YELLOW);
	PrintChar(' ', 25);
	PrintOut("Marwan Muhammad | Youssef Tarek | Muhammad Amr");
	PrintChar(' ', 26);
	PrintOut("|", YELLOW);
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("|", YELLOW);
	PrintChar(' ', 97);
	PrintOut("|", YELLOW);
	PrintChar(' ', 10);
	PrintOut("\n");
	PrintChar(' ', 10);
	PrintOut("+");
	PrintChar('-', 97, YELLOW);
	PrintOut("+");
	PrintChar(' ', 10);
	PrintChar('\n', 2);
}
void Output::ThankYou() {
	PrintChar(' ', 40);
	PrintOut("+");
	PrintChar('-', 38, YELLOW);
	PrintOut("+");
	PrintChar(' ', 40);
	PrintOut("\n");
	PrintChar(' ', 40);
	PrintOut("|", YELLOW);
	PrintChar(' ', 14);
	PrintOut("Thank You!", CYAN);
	PrintChar(' ', 14);
	PrintOut("|", YELLOW);
	PrintChar(' ', 40);
	PrintOut("\n");
	PrintChar(' ', 40);
	PrintOut("+");
	PrintChar('-', 38, YELLOW);
	PrintOut("+");
	PrintChar(' ', 40);
	PrintOut("\n");
}