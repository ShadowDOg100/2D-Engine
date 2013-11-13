#ifndef MSGDATA_H
#define MSGDATA_H

struct MSGDataBytes
{
	char b1,b2,b3,b4;
};
struct MSGDataWords
{
	short lo,hi;
};
union MSGData
{
	int i;
	float f;  
	void *vp;
	MSGDataBytes b;
	unsigned int dw;
	MSGDataWords w;
	MSGData(int ti=0):i(0){}
};


class Message
{
public:
	int sender, target, type, qualifier;
	double delay;
	MSGData p1, p2;
	Message(int from, int to, int msg, int mod, 
		MSGData parm1, MSGData parm2,double d):
			sender(from),target(to), type(msg), 
			qualifier(mod),p1(parm1),p2(parm2),
			delay(d)
	{}
	virtual ~Message()	{}
};
#endif