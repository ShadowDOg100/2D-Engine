#pragma once
//#include "Linklist.h"
#include "../Common/Observer.h"

namespace AICore {
class Trigger:public Observer
{
private:
	float x, y;
	float width, height;
	int spriteID;
	bool active;
	int userInfo;
	float delay;
	//bool touchingTrigger;
	
public:
	Trigger(float tx, float ty, float w, float h, int sID, float d, int ui):
					x(tx),y(ty),width(w),height(h),spriteID(sID),delay(d),userInfo(ui),Observer()
	{}
	virtual ~Trigger(){}

	void SetSprite(int id) { spriteID = id;}
	int GetSprite(){ return spriteID;}

	void Activate( bool stat=true ) { active = stat; }
	bool IsActive() { return active; }
	void Draw();
	void CheckCollision(float tx, float ty, float tw, float th);
	void Send(int to, int type, int qual=0, MSGData p1=0, MSGData p2=0, float delay=0)
		{DISPATCH.Send(ID(), to, type, qual, p1,p2,delay);}
	virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2);
};

class TriggerManager:public Observer
{
private:
	GenArray<Trigger> trigger;
public:
	int CreateTrigger (float x, float y, float width, float height, int spriteID, float delay, int gameSpecific);

	TriggerManager(void);
	virtual ~TriggerManager(void);

	void Update();
	void Draw();
	void CheckCollision(float tx, float ty, float tw, float th);
	void Flush();

};
};