/////////////////////////////////////////////////////////////
// Generic, Template based data storage
//
// This file contains template classes for
//    ListNode (doubly linked list node containing a pointer to data)
//			Next - set and/or get the next node
//			Prev - set and/or get the previous node
//			Data - return the stored pointer to the node's data
//			ClearData - remove the data pointer from the node
//						**** This does not delete the data, just makes
//							it so the node cannot delete it.
//    GenList  (list of ListNodes - including
//			AddAtHead - function to add a node at the top
//			AddAtTail - function to add a node at the bottom
//			GetHead - Return a pointer to the top node.
//			GetTail - return a pointer to the bottom node.
//			Remove - delete the node from the list, healing the list.
//			Count - return the number of items in the list.
//	  Stack - Derived from GenList. Addes the following
//			Push - Adds the data to a node and Calls AddHead
//			Pop - Retrieves the data from the top node, and removes the 
//					node from the list.
//	  Queue - Derived from GenList. Adds the following
//			Add - Adds the data to a node and calls AddTail
//			PeekFront - Returns pointer to data at front of the list
//						but leaves the node (and data) there.
//			GetFront - Returns pointer to data at front of the list
//						and removes ir from the list.
//
//	  GenArray - This class is NOT derived from list
//				This creates an array of pointers to type <AN> (template)
//				It takes a starting size, and increment size
//				Upon destruction, it deletes any data it still holds.
//			Add - This puts the data in the next index, and returns the index.
//					If the array is too small, it is enlarged to accomodate.
//			operator[] - Provides access to the data pointer at index ??
//			ClearData - allows for removing of data from array without deletion.
//						index integrity is maintained.
//			Top- returns the number of items in the array
/////////////////////////////////////////////////////////////

#include <string.h>

#ifndef LINKLIST_H
#define LINKLIST_H

template <class T>
class ListNode
{
protected:
	ListNode<T> *next;
	ListNode<T> *prev;
	T *data;
public:
	ListNode<T>(T *dt=0,ListNode<T> *pr=0, ListNode<T> *n=0 ):
	  data(dt),prev(pr),next(n){}
	virtual ~ListNode<T>()
	{	
		if(data) delete data;
		if(prev) prev->Next(next); 
		if(next) next->Prev(prev);
	}

	ListNode<T> *Next(){return next;}
	ListNode<T> *Prev(){return prev;}
	ListNode<T> *Next(ListNode *n){return next = n;}
	ListNode<T> *Prev(ListNode *p){return prev = p;}
	T* Data(){ return data; }
	void ClearData(){ data = NULL; }
};

template <class LN>
class GenList
{
private:
	ListNode<LN> *head;
	ListNode<LN> *tail;
public:
	GenList():head(0),tail(0){}
	virtual ~GenList()
	{ 
		while(head)
		{
			ListNode<LN> *tmp = head->Next(); 
			delete head; 
			head = tmp; 
		} 
	}
	void AddHead(ListNode<LN> *n)
	{ 
		n->Next(head); 
		if( head ) head->Prev(n); 
		n->Prev(0);
		head=n; 
		if(!tail) tail=n;
	}
	void AddTail(ListNode<LN> *n)
	{ 
		n->Prev(tail); 
		if( tail ) tail->Next(n);
		n->Next(0);
		tail=n; 
		if(!head) head=n;
	}
	void Remove(ListNode<LN> *n)
				{ 
					if(n==head) head=n->Next();
					if(n==tail) tail=n->Prev();
					if(n->Prev()) n->Prev()->Next(n->Next()); 
					if(n->Next()) n->Next()->Prev(n->Prev());
					delete n;
				}
	ListNode<LN> *GetHead(){return head;}
	ListNode<LN> *GetTail(){return tail;}
	int Count()
	{
		int i=0;
		ListNode<LN> *n=head;
		while(n)
		{
			n=n->Next();
			i++;
		}
		return i;
	}
};

template <class SN>
class Stack:public GenList<SN>
{
public:
	Stack():GenList<SN>(){}
	virtual ~Stack(){}
	bool Push(SN *n)
	{
		AddHead(new ListNode<SN>(n));
		return true;
	}
	SN *Pop()
	{
		ListNode<SN> *n = GetHead<SN>();
		SN *val = n->Data<SN>();
		n->ClearData();
		Remove(n);
		return val;
	}
};

template <class QN>
class Queue:public GenList<QN>
{
public:
	Queue():GenList<QN>(){}
	virtual ~Queue(){}
	bool Add(QN *n)
	{
		AddTail(new ListNode<QN>(n));
		return true;
	}
	QN *PeekFront()
	{
		ListNode<QN> *n = GetHead<QN>();
		return n->Data<QN>();
	}
	QN *GetFront()
	{
		ListNode<QN> *n = GetHead<QN>();
		QN *val = n->Data<QN>();
		n->ClearData();
		Remove(n);
		return val;
	}
};



template <class AN>
class GenArray
{
private:
	bool persist;
	bool reuse;
	int size;
	int increment;
	int top;
	AN** data;
	void Grow()
	{
		int curSize=size;
		size+=increment;
		AN** newArray = new AN*[size];
		memcpy(newArray,data,curSize*sizeof(AN*));
		delete [] data;
		data = newArray;
	}
public:
	GenArray<AN>(int sz=10,int inc=5):size(sz),increment(inc),top(0),reuse(false),persist(false)
	{
		data = new AN*[size];
	}
	virtual ~GenArray<AN>()
	{
		if( !data ) return;
		Flush();
		
		//for(int i=0; i<top && i<size; i++)
		//{
		//	if( data[i] && !persist )
		//	{
		//		delete data[i];
		//	}
		//	data[i]=0;
		//}
		delete [] data;
		data=0;
	}
	void ReUse(bool use=true){reuse=use;}
	void SetPersist(bool p){persist=p;}
	int Add(AN* d)
	{
		if( reuse )
		{
			for( int i=0; i<top; i++ )
			{
				if( data[i]==0 )
				{
					data[i]=d;
					return i;
				}
			}
		}
		if( top >= size )
			Grow();
		data[top]=d;
		return top++;
	}
	AN* operator[](int index)
	{
		if( index >= 0 && index < top )
			return data[index];
		return 0;
	}
	void ClearData(int index)
	{
		if( index >= 0 && index < top )
			data[index]=0;
	}
	void Flush()
	{
		if( !data ) return;
		for(int i=0; i<top && i<size; i++)
		{
			if( data[i]!=0 && !persist )
			{
				delete data[i];
			}
			data[i]=0;
		}
		top = 0;
	}
	int Top(){return top;}

};

#endif