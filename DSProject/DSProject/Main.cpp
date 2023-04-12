#include "Process Scheduler/Process Scheduler.h"
int main() 
{
	Scheduler scheduler;
	scheduler.Get_Data();
	cin.ignore();
	while (scheduler.Processing()) 
	{
		scheduler.PrintData();
		getc(stdin);
		system("CLS");
	}
	scheduler.PrintData();
	return 0;
}