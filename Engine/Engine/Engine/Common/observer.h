#ifndef OBSERVER_H
#define OBSERVER_H

#include "messenger.h"

#define ME ID()
class Observer
{
private:
	int id;
public:
	Observer()
		{id=DISPATCH.Register(this);}
	virtual ~Observer()
		{DISPATCH.CheckOut(id);}
	virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2)
		{ return false;}
	void SendMessage(int to, int type, int qual=0, MSGData p1=0, MSGData p2=0, float delay=0)
		{DISPATCH.Send(id, to, type, qual, p1,p2,delay);}
	void RemindMe(float delay,int type,int qual,MSGData p1,MSGData p2)
		{DISPATCH.Send(id, id, type, qual, p1,p2,delay);}
	int ID()
		{return id;}
};

#endif