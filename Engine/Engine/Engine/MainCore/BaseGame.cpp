#include "BaseGame.h"
#include "../IOCore/IOCoreMessage.h"
#include "..\GraphicsCore\GraphicsCore.h"
#include "..\AICore\Messages.h"
#include "MainCoreMessages.h"
#include "MainCore.h"
#include <dinput.h>

#define GM_T_WORLD			1
#define GM_Q_GRAVITY		1

#define GM_T_CHARACTER		2
#define GM_Q_JUMP			1
#define GM_Q_AUTOMOVE		2
#define GM_Q_SPRITE 		3
#define GM_Q_MOVESPEED		4


namespace MainCore {
	enum PlayerSprite{psMoveRight,psMoveLeft,psStandRight,psStandLeft,psJumpRight,psJumpLeft,
		psFallRight,psFallLeft,psENDLIST};
	const int maxPlayerSprites = psENDLIST;
	int mouseX=0, mouseY=0;
	float xveld = 0, yveld = 0;
	bool waitTime=false;
	bool waitHealth = false;
	bool onSpikes = false;
	bool inPit = false;
	int passedHealth = 100;
	float passedTime = 0;
	char *levels[]={"StartScreen","level1","level2","level3","level4","level5"};
	int curLevel=0;
	int characterSprites[maxPlayerSprites];
	bool faceLeft = false;
	float leftSpeed = -80;
	float rightSpeed = 80;

	BaseGame::BaseGame():Observer()
	{
		for(int i=0; i<maxPlayerSprites; i++)
			characterSprites[i]=-1;
	}

	BaseGame::~BaseGame() {}

	void BaseGame::Initialize() {
		a = false;
		d = false;
	}

	void BaseGame::Process() {
		if(curLevel > 0 && curLevel < 4) {
			if(a && !d) {
				character->SetXVel(leftSpeed + xveld);
				faceLeft = true;
				//character->facingFoward = false;
			} else if(d) {
				character->SetXVel(rightSpeed + xveld);
				faceLeft=false;
				//character->facingFoward = true;
			} else {
				character->SetXVel(0 + xveld);
			}
			if( character->GetLastMoveY()<0 )
				character->SetSprite(characterSprites[(faceLeft?psJumpLeft:psJumpRight)]);
			else if( character->GetLastMoveY()>0 )
				character->SetSprite(characterSprites[(faceLeft?psFallLeft:psFallRight)]);
			else
				if( character->GetLastMoveX()==0 )
					character->SetSprite(characterSprites[(faceLeft?psStandLeft:psStandRight)]);
				else
					character->SetSprite(characterSprites[(faceLeft?psMoveLeft:psMoveRight)]);
			float tx,ty;
			character->GetLocation(tx,ty);
			GraphicsCore::MoveCameraTo((int)tx,(int)ty);
		}
	}

	bool BaseGame::Notify(int to, int from, int type, int qual, MSGData p1, MSGData p2) 
	{
		switch(type)
		{
		case IO_T_MOUSE:
			{
				switch( qual )
				{
				case IO_Q_MOUSEMOVE:
					{
						mouseX = p2.w.lo; // position translated to menu relative
						mouseY = p2.w.hi;

					}
				}
			}

		case IO_T_KEYBOARD:
			{
				switch(qual)
				{
				case IO_Q_KEYDOWN:
					{
						if(curLevel > 0) {
							switch( p1.i )
							{
							case DIK_R:
								MainCore::LoadLevel(levels[curLevel]);
								return true;
							case DIK_A:
								a = true;
								p1.i = 5;
								p2.i = 2;
								Send(-1,MC_T_HUDCHANGE,MC_Q_START,p1,p2,0);
								return true;
							case DIK_D:
								d = true;
								p1.i = 5;
								p2.i = 2;
								Send(-1,MC_T_HUDCHANGE,MC_Q_START,p1,p2,0);
								return true;
							case DIK_SPACE:
								character->Jump();
								p1.i = 5;
								p2.i = 2;
								Send(-1,MC_T_HUDCHANGE,MC_Q_START,p1,p2,0);
								return true;
								//case DIK_Q:
								//	p1.i = 1;
								//	p2.i = -2;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_INCREMENT,p1,p2,0);
								//	return true;
								//case DIK_E:
								//	p1.i = 1;
								//	p2.i = 2;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_INCREMENT,p1,p2,0);
								//	return true;
								//case DIK_S:
								//	p1.i = 5;
								//	p2.i = 2;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_STOP,p1,p2,0);
								//	return true;
								//case DIK_G:
								//	p1.i = 5;
								//	p2.i = 2;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_START,p1,p2,0);
								//	return true;
								//case DIK_C:
								//	p1.i = 5;
								//	p2.f = -480;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_SET,p1,p2,0);
								//	p2.w.lo = -1;
								//	p2.w.hi=10000;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_SETRANGE,p1,p2,0);
								//	return true;
								//case DIK_V:
								//	p1.i = 5;
								//	p2.f = 0.f;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_SET,p1,p2,0);
								//	p2.w.lo = -1;
								//	p2.w.hi=20000;
								//	Send(-1,MC_T_HUDCHANGE,MC_Q_SETRANGE,p1,p2,0);
								//	return true;
							};
						}
					}
				case IO_Q_KEYRELEASE:
					{
						//if(levelName == "level1") {
						switch( p1.i )
						{
						case DIK_A:
							a = false;
							return true;
						case DIK_D:
							d = false;
							return true;
						};
						//}
					}
				};
			}
			break;
		case IO_T_CONTROLCHANGE:
			switch( p2.i )
			{
			case 1:	// health bar
				// onSpikes is true if on spikes
				// inPit is true if fell in the pit
				// both are false if warp door
				passedHealth = p1.i;
				if( waitHealth ) // health bar at zero
				{
					waitHealth = false;
					if( passedHealth == 0 ) 
					{
						curLevel = 4;
						MainCore::LoadLevel(levels[curLevel]);
						a=false;
						d=false;
						return true;
					}
					waitTime = true;
					p1.i=5;
					Send(-1,MC_T_HUDCHANGE,MC_Q_INQUIRE,p1,p2,0); 
				}
				//waitHealth = false;
				//waitTime = true;
				////passedTime = p1.f;
				//if( passedHealth == 0 )
				//{
				//	MainCore::LoadLevel("level1");
				//	passedTime  = 0;
				//	passedHealth = 100;
				//}
				break;
			case 5: // game timer
				if( waitTime )
				{
					a=false;
					d=false;
					waitTime = false;
					passedTime = p1.f;
					if( onSpikes )
					{
						onSpikes = false;
					}
					else // in pit or warp door
					{
						inPit = false;
						MainCore::LoadLevel(levels[curLevel]);
						p1.i = 5;
						p2.f = passedTime;
						Send(-1,MC_T_HUDCHANGE,MC_Q_SET,p1,p2,0);
						p2.w.lo = -1;
						p2.w.hi=32767;
						Send(-1,MC_T_HUDCHANGE,MC_Q_SETRANGE,p1,p2,0);
						p1.i=1;
						p2.w.lo=0;
						p2.w.hi=100;
						Send(-1,MC_T_HUDCHANGE,MC_Q_SETRANGE,p1,p2,0);
						p2.i=passedHealth;
						Send(-1,MC_T_HUDCHANGE,MC_Q_SET,p1,p2,0);
					}

				}
				break;
			}
			break;
		case IO_T_MENU_BUTTON:
			{
				switch(qual)
				{
				case IO_Q_BUTTON_CLICK:
					{
						switch(p2.i)
						{
						case 1:
							curLevel = 1;
							MainCore::LoadLevel(levels[curLevel]);
							return true;
							//case 2:
							//	if( MainCore::IsGameClockRunning() )
							//	{
							//		MainCore::PauseGameClock();
							//	}
							//	else
							//	{
							//		MainCore::RunGameClock();
							//	}
							//	return true;
						};
						break;
					}
				};
			}
			break;
		case AI_T_Trigger:
			{
				switch(qual) 
				{
				case AI_Q_TriggerTripped:
					{
						switch(p2.i)
						{
						case 1: // death occured from  bottomless pit
							onSpikes = false;
							inPit = true;
							waitHealth = true;
							p1.i=1;					
							p2.i=-25;
							Send(-1,MC_T_HUDCHANGE,MC_Q_INCREMENT,p1,p2,0);
							break;					
						case 2: // hit a collectible hour glass
							p1.i = 5;
							p2.f = -3;
							Send(-1,MC_T_HUDCHANGE,MC_Q_INCREMENT,p1,p2,0);						
							break;
						case 3: // death occured from spikes
							onSpikes = true;
							inPit = false;
							waitHealth = true;
							p1.i=1;
							p2.i=-25;
							Send(-1,MC_T_HUDCHANGE,MC_Q_INCREMENT,p1,p2,0); 
							break;
						case 4: // Warp door - go to next level
							waitHealth = true;
							onSpikes = false;
							inPit = false;
							curLevel++;
							if( curLevel > 3 ) curLevel = 0;
							p1.i=1;					
							Send(-1,MC_T_HUDCHANGE,MC_Q_INQUIRE,p1,p2,0);
						}
					}
					break;
				}
			}
			break;
		case GM_T_WORLD:
			{
				switch(qual) 
				{
				case GM_Q_GRAVITY:
					{
						character->SetGravity(p1.f);
					}
					break;
				}
			}
			break;
		case GM_T_CHARACTER:
			{
				switch(qual) 
				{
				case GM_Q_JUMP:
					{
						character->SetJumpSpeed(p1.f);
					}
					return true;
				case GM_Q_AUTOMOVE:
					{
						xveld = p1.f;
						yveld = p2.f;
					}
					return true;
				case GM_Q_SPRITE:
					{
						if( p1.i < maxPlayerSprites )
							characterSprites[p1.i] = p2.i;
					}
					return true;
				case GM_Q_MOVESPEED:
					leftSpeed = p1.f;
					rightSpeed = p2.f;
					return true;
				}

				break;
			}
			break;
		};
		return false;
	}

	void BaseGame::Draw() {
		//char str[128];
		//sprintf_s(str,128,"MousePos(%d,%d)",mouseX,mouseY);
		//GraphicsCore::DrawText(str,300,25,200,50,0,255,255,255);
		//sprintf_s(str,128,"MouseScale(%.04f,%.04f)",GraphicsCore::MouseScaleX(),GraphicsCore::MouseScaleY());
		//GraphicsCore::DrawText(str,300,75,200,50,0,0,255,255);
		//if(levelName == "level1") {
		//	sprintf_s(str,128,"Player Velocity X: %.04f Y: %.04f",character->GetXVel(),character->GetYVel());
		//	GraphicsCore::DrawText(str,300,175,200,50,0,0,255,255);
		//}
		//double gameTime = MainCore::GetGameTime();
		//int minutes = (int)gameTime/60;
		//double seconds = (gameTime/60 - (int)gameTime/60)*60.0;
		//if( seconds < 10. )
		//	sprintf_s(str,128,"Game Clock: %02d:0%.2f",minutes,seconds);
		//else
		//	sprintf_s(str,128,"Game Clock: %02d:%.2f",minutes,seconds);
		//GraphicsCore::DrawText(str,300,125,200,50,0,0,255,255);
	}

	void BaseGame::Shutdown() {
	}

	void BaseGame::SetCharacter(AICore::Character* c) {
		character = c;
	}

	void BaseGame::SetLevel(std::string level) {
		levelName = level;
		if(levelName == "level1") {
			if(!MainCore::IsGameClockRunning()) {
				MainCore::RunGameClock();
			}
			character = MainCore::GetCharacter(0);
		}
	}
};