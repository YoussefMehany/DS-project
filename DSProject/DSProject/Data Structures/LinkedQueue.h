#pragma once
#include "Node.h"
template <typename T>
class Queue
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int Counter;
public:
	Queue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int GetSize()const;
	void Print() const;
	~Queue();

	//copy constructor
	Queue(const Queue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
Queue<T>::Queue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
	Counter = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
bool Queue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}
/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
int Queue<T>::GetSize()const 
{
	return Counter;
}

/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Queue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	Counter++;
	return true;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////



template <typename T>
bool Queue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
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
bool Queue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T>::~Queue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
Queue<T>::Queue(const Queue<T>& LQ)
{
	Node<T>* NodePtr = LQ.frontPtr;
	frontPtr = backPtr = nullptr;
	while (NodePtr) {
		enqueue(NodePtr->getItem());
		NodePtr = NodePtr->getNext();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Queue<T>::Print() const
{
	Node<T>* temp = frontPtr;
	cout << "[ ";
	for (size_t i = 0; i < Counter; i++, temp = temp->getNext())
		cout << temp->getItem() << " ";
	cout << "]" << endl;
}
