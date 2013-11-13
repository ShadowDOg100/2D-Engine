#include "InputManager.h"
#include "../MainCore/MainCoreMessages.h"
#include "../MainCore/MainCore.h"
#include "IOManager.h"
#include "TimerControl.h"
#include "IOCore.h"

namespace IOCore
{
	Timer::Timer(int ix, int iy, int iw, int ih, const char *txt, int bID, int user) :
				MenuControl(ix, iy, iw, ih, user), backgroundID(bID), 
				running(false),curTime(0),
				minutes(0),seconds(0),startValue(0),countDown(false),
				minTime(-.1f),maxTime(35000)
	{
		if( txt && strlen(txt)>0 )
		{
			strcpy_s(text,128,txt);
		}
		else
		{
			text[0]=0;
		}
		int valCount = 0;
		for(int i = 0; i<(int)strlen(text); i++)
		{
			if( text[i]=='%' ) valCount++;
		}
		startTime = MainCore::GetGameTime();
		displayMinutes = valCount>1?true:false;
		RemindMe(.025f,-1,-1,0,0);
	}
	void Timer::Draw(int dx, int dy)
	{
		if(backgroundID >= 0)
			GraphicsCore::DrawSprite(backgroundID, w, h, x+dx, y+dy);

		char str[128];
		if( strlen(text) > 0 )
		{
			if( displayMinutes )
				sprintf_s(str,128,text,minutes,seconds);
			else
				sprintf_s(str,128,text,curTime);
		}
		else
			strcpy_s(str,128,"Unformatted");
		GraphicsCore::DrawText(str,x+dx,y+dy,w,h,255,255,255,255);
	}
	bool Timer::Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2)
	{
		if( to == from && to == ID() )
		{
			RemindMe(.025f,-1,-1,0,0);
			UpdateTime();
			return true;
		}
		switch(type) 
		{
		case MC_T_HUDCHANGE:
			{
				if(p1.i != userInfo) return false;
				switch(qual)
				{
				case MC_Q_SET:
					{
						startValue = (double)(p2.f);
						countDown = startValue<0?true:false;
						startTime = MainCore::GetGameTime()-startValue;
						curTime = startValue<0?-startValue:startValue;
						return true;
					}
				case MC_Q_INCREMENT:
					{
						if( countDown )
							startTime += (double)(p2.f);
						else
							startTime -=  (double)(p2.f);
						return true;
					}
				case MC_Q_INQUIRE:
					{
						MSGData p1t, p2t;
						p1t.f = (float)curTime;
						p2t.i = userInfo;
						Send(-1,IO_T_CONTROLCHANGE,-1,p1t,p2t,0);
						return true;
					}
				case MC_Q_START:
					{
						running = true;
						return true;
					}
				case MC_Q_RESET:
					{	
						startTime = MainCore::GetGameTime()-startValue;
						curTime = startValue<0?-startValue:startValue;
						return true;
					}
				case MC_Q_STOP:
					{
						running=false;
						return true;
					}
				case MC_Q_SETRANGE:
					{
						minTime = p2.w.lo;
						maxTime = p2.w.hi;
					}
				}
			}
			break;
		}
		return false;
	}
	void Timer::UpdateTime()
	{
		double curGameTime = MainCore::GetGameTime();
		if( running )
		{
			if( countDown )
				curTime = startTime - curGameTime;
			else
				curTime = curGameTime - startTime;
		}
		else
		{
			if( countDown )
				startTime = curGameTime + curTime;
			else
				startTime = curGameTime - curTime;
		}
		minutes = (int)(curTime / 60);
		seconds = (float)(curTime - (double)minutes*60);
		if( curTime <= minTime || curTime >= maxTime )
		{
			running = false;
			MSGData p1t, p2t;
			p1t.f = (float)curTime;
			p2t.i = userInfo;
			Send(-1,IO_T_CONTROLCHANGE,-1,p1t,p2t,0);
		}
	}

};