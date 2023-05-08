#pragma once
#include<iostream>
using namespace std;
//First let's declare a single node in the list
template<typename T>
class TreeNode
{
private:
	T item;	// A data item (can be any complex sturcture)
	TreeNode<T>* left, *right;	// Pointer to next node
public:

	TreeNode() //default constructor
	{
		left = right = nullptr;
	}

	TreeNode(T newItem) //non-default constructor
	{
		item = newItem;
		left= right = nullptr;
	}

	void setItem(T newItem)
	{
		item = newItem;
	} // end setItem

	void setLeft(TreeNode<T>* nextNodePtr)
	{
		left = nextNodePtr;
	} // end setNext
	void setRight(TreeNode<T>* nextNodePtr)
	{
		right = nextNodePtr;
	}
	T getItem() const
	{
		return item;
	} // end getItem

	TreeNode<T>* getLeft() const
	{
		return left;
	}
	TreeNode<T>* getRight() const
	{
		return right;
	}
}; // end Node