#pragma once
using namespace std;
template<typename T>
class Pri_Node
{
private:
	int Priority;
	T item;	// A data item (can be any complex sturcture)
	Pri_Node<T>* next;	// Pointer to next node
public:

	Pri_Node() //default constructor
	{
		next = nullptr;
	}

	Pri_Node(T newItem,int Pri) //non-default constructor
	{
		Priority = Pri;
		item = newItem;
		next = nullptr;
	}

	void setPriority(int Pri)
	{
		Priority = Pri;
	} // end setPriority

	void setItem(T newItem)
	{
		item = newItem;
	} // end setItem

	void setNext(Pri_Node<T>* nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext

	int getPriority() const
	{
		return Priority;
	} // end getPriority

	T getItem() const
	{
		return item;
	} // end getItem

	Pri_Node<T>* getNext() const
	{
		return next;
	}
}; // end Node