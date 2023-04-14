#include "Process Scheduler/Process Scheduler.h"
int main() 
{
	Scheduler scheduler;
	scheduler.Get_Data();
	int x;
	cout << "Please Enter The Mode of The Interface :\n";
	cout << "1.Interactive Mode\n2.Step-By-Step Mode\n3.Silent Mode\n";
	cin >> x;
	scheduler.Set_Mode(InterfaceMode(x - 1));
	cin.ignore();
	system("CLS");
	while (scheduler.Processing()) 
		scheduler.UpdateInterface();
	return 0;
}