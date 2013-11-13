#include <windows.h>
#include <cassert>
#include "observer.h"

// get current time (seconds) from startup
double GetTime()
{
	static bool init=true;
	static double timeScale=1.0;
	if( init )
	{
		LONGLONG perfCountFreq;
		QueryPerformanceFrequency( (LARGE_INTEGER*) &perfCountFreq);
		timeScale = 1.0/perfCountFreq;
		init = false;
	}
	LONGLONG curTime;
	QueryPerformanceCounter( (LARGE_INTEGER*) &curTime);
	return curTime*timeScale;

}
Dispatch* Dispatch::Instance()
{
	static Dispatch *instance = new Dispatch();
	return instance;
}
void Dispatch::DispatchMsg(Message *msg)
{
	if( msg->target >= 0 )
	{
		if( contacts[msg->target])
		{
			contacts[msg->target]->Notify(msg->sender,msg->target,msg->type,msg->qualifier,msg->p1,msg->p2);
		}
		return;
	}
	for(int i=0; i<contacts.Top(); i++)
	{
		if( contacts[i])
		{
			if(contacts[i]->Notify(msg->sender,msg->target,msg->type,msg->qualifier,msg->p1,msg->p2))
				return;
		}
	}
}
ListNode<Message>* Dispatch::RemoveMessage(ListNode<Message> *n)
{
	ListNode<Message> *next = n->Next();
	msgs.Remove(n);
	return next;
}
void Dispatch::Send(int from,int to,int type,int qual, 
				MSGData p1,MSGData p2,float delay)
{
	double tc = GetTime()+delay;
	msgs.AddTail(new ListNode<Message>(new Message(from,to,type,qual,p1,p2,tc)));
}
void Dispatch::Process()
{
	double tc = GetTime();
	ListNode<Message>*n = msgs.GetHead();
	while(n)
	{
		if( n->Data()->delay<=tc )
		{
			DispatchMsg(n->Data());
			n=RemoveMessage(n);
		}
		else
		{
			n=n->Next();
		}
	}
}
