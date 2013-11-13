#ifndef MAINCOREMEAGING_H_
#define MAINCOREMEAGING_H_

#include "../Common/observer.h"
#include "BaseGame.h"

namespace MainCore {
	BaseGame *game;

	class MainCoreMessaging : public Observer {
	public:
		MainCoreMessaging() : Observer() {}
		virtual ~MainCoreMessaging() {}
		virtual bool Notify(int to, int from, int type, int qual, MSGData p1, MSGData p2);
		void Send(int to, int type, int qual=0, MSGData p1=0, MSGData p2=0, float delay=0)
		{DISPATCH.Send(ID(), to, type, qual, p1,p2,delay);}
	};

	MainCoreMessaging *MCMessage;
};

#endif