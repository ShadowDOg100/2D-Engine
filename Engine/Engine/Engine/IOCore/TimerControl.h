#ifndef TIMERCONTROL_H
#define TIMERCONTROL_H
#include "menu.h"

namespace IOCore
{

	class Timer : public MenuControl
	{
	private:
		int backgroundID;
		char text[128];
		bool running;
		double startTime;
		double curTime;
		double startValue;
		bool countDown;
		bool displayMinutes;
		int minutes;
		float seconds;
		float minTime;
		float maxTime;

	public:
		Timer(int ix, int iy, int iw, int ih, const char *txt, int bID, int user);
		virtual ~Timer(){}

		virtual void Draw(int dx, int dy);
		virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2);
		void UpdateTime();

	};
};

#endif