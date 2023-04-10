#include "Process Scheduler/Process Scheduler.h"
int main() 
{
	Scheduler scheduler;
	scheduler.Get_Data();
	while (scheduler.Processing()) 
	{
		scheduler.PrintData();
		getc(stdin);
	}
	return 0;
}