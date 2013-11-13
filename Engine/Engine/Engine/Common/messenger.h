#ifndef MESSENGER_H
#define MESSENGER_H

#include "linklist.h"
#include "msgdata.h"
class Observer;
#define DISPATCH (*(Dispatch::Instance()))

class Dispatch
{
private:
	GenArray<Observer> contacts;
	GenList<Message> msgs;
	Dispatch(){contacts.ReUse();}
	void DispatchMsg(Message *msg);
	ListNode<Message>* RemoveMessage(ListNode<Message> *n);
public:
	static Dispatch *Instance();
	void Send(int from,int to,int type,int qual, 
				MSGData p1,MSGData p2,float delay);
	void Process();
	int Register(Observer *o){return contacts.Add(o);}
	void CheckOut(int id){contacts.ClearData(id);}
};

double GetTime();

#endif
