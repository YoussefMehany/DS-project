#pragma once
#include "Data Structures/CircularLinkedList.h"
#include "Data Structures/LinkedQueue.h"
#include "Data Structures/Pri_Queue.h"
#include "Process/Process.h"
#include "Processor/Processor.h"
class Schedular
{
	Queue<Process*> NEW, BLK, TRM; // New , BLK , TRM Lists
	List<Processor*> MultiProcessor; // Processors List

public:

};