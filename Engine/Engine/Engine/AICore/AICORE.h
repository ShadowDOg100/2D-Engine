#ifndef AICORE_H_
#define AICORE_H_

#include "Character.h"

namespace AICore
{
    void Initialize();
    void Shutdown();
    void Draw();
    void Process();
	int CreatePlayerCharacter(bool npc, int spriteId, float x, float y, float width, float height, int gameSpecificId); 
	int CreateObstacle(int spriteId, float x, float y, float w, float h, bool solid, int gameSpecificId);
	int CreateTrigger(int spriteId, float x, float y, float w, float h, float delay, int gameSpecificId);
	void ClearLevel();
	Character* GetCharacter(int ID);
};

#endif 
