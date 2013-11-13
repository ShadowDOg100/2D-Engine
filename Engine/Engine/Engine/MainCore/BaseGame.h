#ifndef BASE_GAME_H_
#define BASE_GAME_H_

#include "../Common/Observer.h"
#include "../AICore/Character.h"
#include <string>

namespace MainCore {
class BaseGame : public Observer {
private:
	AICore::Character* character;
	std::string levelName;
	bool a, d;
public:
	BaseGame (void);
	virtual ~BaseGame(void);

	void Initialize ();
	void Process ();
	void Draw (); 
	void Shutdown ();
	void SetCharacter (AICore::Character* character);
	void SetLevel(std::string level);

	virtual bool Notify(int to, int from, int type, int qual, MSGData p1, MSGData p2);
	void Send(int to, int type, int qual=0, MSGData p1=0, MSGData p2=0, float delay=0)
		{DISPATCH.Send(ID(), to, type, qual, p1,p2,delay);}
};
};

#endif