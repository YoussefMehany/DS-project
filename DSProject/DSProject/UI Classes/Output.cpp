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
			Runlist += to_string(Multiprocessors[i]->Get_Run()->GetPID()) + " (P"+to_string(i+1)+"), ";
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
void Output::PrintColor(Colors clr) {
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
void Output::PrintReadme() {
	Table readme;
	readme.format().border_color(Color::yellow);

	readme.add_row(Row_t{ "Process Scheduler" });
	readme[0].format().font_align(FontAlign::center).font_color(Color::yellow);

	readme.add_row(Row_t{ "https://github.com/YoussefMehany/DS-project" });
	readme[1]
		.format()
		.font_align(FontAlign::center)
		.font_style({ FontStyle::underline, FontStyle::italic })
		.font_color(Color::white)
		.hide_border_top();

	readme.add_row(Row_t{ "This is the project of CMP1040 (Data Structures and Algorithms) course , The contributors are :" });
	readme[2].format().font_style({ FontStyle::italic }).font_color(Color::magenta);

	Table highlights;
	highlights.add_row(Row_t{ "Marwan Muhammad", "Youssef Tarek", "Muhammad Amr" });
	readme.add_row(Row_t{ highlights });
	readme[3].format().font_align(FontAlign::center).hide_border_top();
	cout << readme<<'\n';
}