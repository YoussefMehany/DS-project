#pragma once
#include "Pri_Node.h"
template <typename T>
class PriorityQueue
{
private:

	Pri_Node<T>* backPtr;
	Pri_Node<T>* frontPtr;
	int Counter;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, int Priority);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int getSize() const;
	void Print() const;
	~PriorityQueue();

	//copy constructor
	PriorityQueue(const PriorityQueue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	Counter = 0;
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
int PriorityQueue<T>::getSize() const
{
	return Counter;
}


template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, int Priority)
{
	Pri_Node<T>* newNodePtr = new Pri_Node<T>(newEntry, Priority), * temp = frontPtr, * prev = nullptr;
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		backPtr = frontPtr = newNodePtr; // The queue is empty
	else  // The queue was not empty
	{
		while (temp && temp->getPriority() <= Priority)
		{
			prev = temp;
			temp = temp->getNext();
		}
		newNodePtr->setNext(temp);

		if (!prev)
			frontPtr = newNodePtr;

		else
			prev->setNext(newNodePtr);

		if (!temp)
			backPtr = newNodePtr;
	}

	Counter++;
	return true;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////



template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Pri_Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	Counter--;
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool PriorityQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& LQ)
{
	Pri_Node<T>* NodePtr = LQ.frontPtr;
	frontPtr = backPtr = nullptr;
	Counter = 0;
	while (NodePtr) {
		enqueue(NodePtr->getItem());
		NodePtr = NodePtr->getNext();
	}
}

template <typename T>
void PriorityQueue<T>::Print() const
{
	Pri_Node<T>* temp = frontPtr;
	for (size_t i = 0; i < Counter; i++, temp = temp->getNext()) {
		cout << *temp->getItem();
		if (i < Counter - 1) cout << ", ";
	}
	cout << endl;
}

