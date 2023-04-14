#pragma once
#include"Node.h"
template <class T>
class CList
{
	Node<T>* Head;
	int GetLen(Node<T>* head) const;
	int Counter;
public:
	CList() { Head = NULL; Counter = 0; }
	int getSize() const { return Counter; };
	bool Empty()const { return !Head; }
	int Size()const { return GetLen(Head); }
	T HeadItem() { return Head->getItem(); }
	void InsertBeg(T Val)
	{
		Node<T>* temp = new Node<T>(Val), * tail = Head;
		while (tail && tail->getNext() != Head)
			tail = tail->getNext();
		temp->setNext(Head);
		if (!Head)
			temp->setNext(temp);
		else
			tail->setNext(temp);
		Head = temp;
		Counter++;
	}
	void InsertEnd(T Val)
	{

		Node<T>* temp = new Node<T>(Val);
		if (!Head) {
			Head = temp;
			Head->setNext(Head);
			return;
		}
		Node<T>* tail = Head;
		while (tail->getNext() != Head)
			tail = tail->getNext();
		tail->setNext(temp);
		temp->setNext(Head);
		Counter++;
	}
	bool RemoveBeg()
	{
		if (!Head)
			return false;
		Node<T>* tail = Head, * temp2 = Head;
		while (tail->getNext() != Head)
			tail = tail->getNext();
		if (Head == Head->getNext()) {
			delete Head;
			Head = nullptr;
		}
		else {
			Head = Head->getNext();
			tail->setNext(Head);
			delete temp2;
		}
		Counter--;
		return true;
	}
	bool Remove(const T& Element)
	{
		if (!Head)
			return false;
		if (Element == Head->getItem())
			RemoveBeg();
		else
		{
			Node<T>* temp = Head, * bye = nullptr;
			while (temp->getNext()->getItem() != Element) {
				if (temp->getNext() == Head)
					return false;
				temp = temp->getNext();
			}
			bye = temp->getNext();
			temp->setNext(bye->getNext());
			delete bye;
		}
		Counter--;
		return true;
	}
	void Print()const
	{
		Node<T>* temp = Head;
		if (!temp)
			return;
		while (temp->getNext() != Head)
		{
			cout << *temp->getItem() << ", ";
			temp = temp->getNext();
		}
		cout << *temp->getItem() << endl;

	}
};
template<class T>
int CList<T>::GetLen(Node<T>* head) const
{
	if (!head)
		return 0;
	if (head->getNext() == Head)
		return 1;
	return 1 + GetLen(head->getNext());
}