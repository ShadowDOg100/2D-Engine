#include "AICORE.h"
#include "Messages.h"
#include "Obstacle.h"
#include "Character.h"
#include "Trigger.h"

namespace AICore
{
	ObstacleManager *obstacles=0;
	CharacterManager *characters=0;
	TriggerManager *triggers=0;

    void Initialize()
	{
		obstacles = new ObstacleManager();
		characters = new CharacterManager();
		triggers = new TriggerManager();
	}
 
    void Shutdown()
	{
		if( obstacles ) {
			obstacles->Flush();
			delete obstacles;
		}
		obstacles = 0;

		if(characters) {
			characters->Flush();
			delete characters;
		}
		characters = 0;

		if(triggers) {
			triggers->Flush();
			delete triggers;
		}
		triggers = 0;
	}
 
    void Draw()
	{
		obstacles->Draw();
		characters->Draw();
		triggers->Draw();
	}
 
    void Process()
	{
		characters->ProcessPhysics();

		// Check player character against obstacles for collisions
		characters->CheckObstacleCollisions(obstacles);

		// Check player character against triggers for collisions
		characters->CheckTriggerCollisions(triggers);
	}
 
	int CreatePlayerCharacter(bool npc, int spriteID, float x, float y, float width, float height, int gameSpecificId)
	{
		return characters->CreateCharacter(x,y,width,height, npc, spriteID, gameSpecificId);
	}
 
	int CreateObstacle(int spriteId, float x, float y, float w, float h, bool solid, int gameSpecificId)
	{
		return obstacles->CreateObstacle(x,y,w,h,solid,spriteId,gameSpecificId);
	}
 
	int CreateTrigger(int spriteId, float x, float y, float w, float h, float delay, int gameSpecificId)
	{
		return triggers->CreateTrigger(x,y,w,h,spriteId, delay, gameSpecificId);
	}

	void ClearLevel() {
		obstacles->Flush();
		characters->Flush();
		triggers->Flush();
	}

	Character* GetCharacter(int ID) {
		return characters->GetCharacter(ID);
	}
};
