#pragma once
#include "Node.h"
template <class T>
class List
{
	Node<T>* Head, * Tail;
	int Size;
public:
	List() { Head = Tail = nullptr; Size = 0; }
	List(const List<T>& L)
	{
		Head = Tail = nullptr;
		Size = 0;
		Node<T>* head = L.Head;
		while (head)
		{
			InsertEnd(head->getItem());
			head = head->getNext();
		}
	}
	~List() {
		T temp;
		while (RemoveHead(temp));
	}
	bool Empty()const { return Size == 0; }
	int size()const { return Size; }
	T head()const { return Head->getItem(); }
	T tail()const { return Tail->getItem(); }
	void InsertBeg(T Val)
	{
		Node<T>* temp = new Node<T>(Val);
		temp->setNext(Head);
		Head = temp;
		if (!Size)
			Tail = Head;
		Size++;
	}
	void InsertEnd(T Val)
	{
		Node<T>* temp = new Node<T>(Val);
		if (!Size)
			Head = Tail = temp;
		else
		{
			Tail->setNext(temp);
			Tail = temp;
		}
		Size++;
	}
	void GetItem(int Pos, T& Get)
	{
		if (Pos < 0 || Pos >= Size)
			return;
		Node<T>* temp = Head;
		for (int i = 0; i < Pos; i++)
			temp = temp->getNext();
		Get = temp->getItem();
	}
	bool RemoveHead(T& Get)
	{
		if (Size)
		{
			Node<T>* temp = Head;
			Head = Head->getNext();
			if (Size == 1)
				Tail = Head;
			Get = temp->getItem();
			delete temp;
			Size--;
			return true;
		}
		return false;
	}
	bool Remove(int Pos, T& Get)
	{
		if (Pos < 0 || Pos >= Size)
			return false;
		if (Pos == 0)
			RemoveHead(Get);
		else {
			Node<T>* temp = Head, * prev = nullptr;
			for (int i = 0; i < Pos; i++)
			{
				prev = temp;
				temp = temp->getNext();
			}
			prev->setNext(temp->getNext());
			if (Pos == Size - 1) Tail = prev;
			Size--;
			Get = temp->getItem();
			delete temp;
		}
		return true;
	}
	void print()const;
};
template<class T>
inline void List<T>::print()const
{
	Node<T>* temp = Head;
	while (temp)
	{
		cout << temp->getItem();
		temp = temp->getNext();
		if (temp) cout << ", ";
	}
	cout << endl;
}
template<>
inline void List<Process*>::print()const
{
	Node<Process*>* temp = Head;
	while (temp)
	{
		cout << *temp->getItem();
		temp = temp->getNext();
		if (temp) cout << ", ";
	}
	cout << endl;
}