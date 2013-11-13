#pragma once 
//#include "Linklist.h"
#include "../Common/Observer.h"

namespace AICore {
class Obstacle : public Observer
{
private:
	float x, y;
	float width, height;
	bool solid;
	int spriteID;
	int userInfo;
public:
	Obstacle(float tx, float ty, float w, float h, bool isSolid, int sID, int gS):
					x(tx),y(ty),width(w),height(h),solid(isSolid),spriteID(sID),userInfo(gS),Observer()
	{}
	~Obstacle(){}

	void SetSprite(int id) { spriteID = id;}
	int GetSprite(){ return spriteID;}
	void MakeCollide(bool state) { solid = state; }
	void CheckCollision(float &tx, float &ty, float tw, float th, float &vx, float &vy, bool* sides);
	bool IsSolid() { return solid; }
	void Draw();
};

class ObstacleManager : public Observer
{
private:
	GenArray<Obstacle> obstacle;
public:
	int CreateObstacle (float x, float y, float width, float height, bool solid, int spriteID, int gameSpecific);

	ObstacleManager(void);
	~ObstacleManager(void);

	void Draw();
	void CheckCollision(float &tx, float &ty, float tw, float th, float &vx, float &vy, bool* sides);
	void Flush();
};
};