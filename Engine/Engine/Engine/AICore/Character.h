#pragma once
#include "../Common/Observer.h"
#include "Vector2D.h"

namespace AICore {
class Character:public Observer
{
private:
	Vector2D pos;
	Vector2D vel;
	Vector2D acel;
	Vector2D lastMove;
	bool grounded;
	float jumpSpeed;

	float width, height;
	int spriteID;
	bool npc;
	int userInfo;
public:
	bool facingFoward;

	Character(float tx, float ty, float w, float h, bool n, int sID, int gS):
					pos(Vector2D(tx,ty)),vel(Vector2D(0,0)),acel(Vector2D(0,0)),jumpSpeed(0),width(w),height(h),spriteID(sID),userInfo(gS),npc(n),Observer(),grounded(false)
	{}
	virtual ~Character(){}

	void SetSprite(int id) { spriteID = id;}
	int GetSprite(){ return spriteID;}
	void Draw();
	void GetLocation(float &tx, float &ty)
	{
		tx=pos[0]; ty=pos[1]; return;
	}
	void GetSize(float &tw, float &th)
	{
		tw = width; th = height; return;
	}
	bool IsNPC(){ return npc; }
	void ProcessPhysics(float d_t);
	void Move(float ix, float iy) { pos[0] += ix; pos[1] += iy; }
	void MoveTo(float tx, float ty) { pos = Vector2D(tx, ty); }
	void SetXVel(float x) { vel[0] = x; }
	void SetYVel(float y) { vel[1] = y; }
	float GetXVel() { return vel[0]; }
	float GetYVel() { return vel[1]; }
	float GetLastMoveX(){ return lastMove[0]; }
	float GetLastMoveY(){ return lastMove[1]; }
	void SetLastMoveX(float x) { lastMove[0] = x; }
	void SetLastMoveY(float y) { lastMove[1] = y; }
	void SetGrounded(bool g) { grounded = g; }
	void Jump();	
	void SetGravity(float g) { acel[1] = g; }
	void SetJumpSpeed(float s) { jumpSpeed = s; }
};

class TriggerManager;
class ObstacleManager;

class CharacterManager
{
private:
	GenArray<Character> character;
	double lastUpdate;
public:
	int CreateCharacter (float x, float y, float width, float height, bool npc, int spriteID, int gameSpecific);

	CharacterManager(void);
	~CharacterManager(void);

	void Draw();

	void ProcessPhysics();

	void CheckTriggerCollisions(TriggerManager *triggers);
	void CheckObstacleCollisions(ObstacleManager *obstacles);
	bool CheckPosition(int id, int x, int y);
	void Flush();
	Character* GetCharacter(int ID);
};
};
