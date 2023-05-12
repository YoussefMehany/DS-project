#include "Process Scheduler/Process Scheduler.h"
int main() 
{
	Scheduler scheduler;
	scheduler.Get_Data();
	scheduler.UpdateInterface();
	while (scheduler.Simulation())
		scheduler.UpdateInterface();
	return 0;
}