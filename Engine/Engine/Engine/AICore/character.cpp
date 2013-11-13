#include "Character.h"
#include "Trigger.h"
#include "Obstacle.h"
#include "../GraphicsCore/GraphicsCore.h"
#include "../MainCore/MainCore.h"

namespace AICore {
int CharacterManager::CreateCharacter (float x, float y, float width, float height, bool npc, int spriteID, int gameSpecific)
{
	return character.Add(new Character(x, y, width, height, npc, spriteID, gameSpecific));
}

CharacterManager::CharacterManager(void)
{
	lastUpdate = GetTime();
}
CharacterManager::~CharacterManager(void)
{
}

void CharacterManager::Draw()
{
	for(int i = 0; i < character.Top(); i++) {
		if(character[i])
			character[i]->Draw();
	}
}

void CharacterManager::ProcessPhysics() {
	if(MainCore::IsGameClockRunning()) {
		double d_t = GetTime() - lastUpdate;
		for(int i = 0; i < character.Top(); i++) {
			if(character[i] && !(character[i]->IsNPC())) {
				character[i]->ProcessPhysics((float)d_t);
			}
		}
	}
	lastUpdate = GetTime();
}

void CharacterManager::CheckTriggerCollisions(TriggerManager *triggers)
{
	for(int i=0; i<character.Top(); i++)
		if( character[i] && !(character[i]->IsNPC()) )
		{
			float x, y, w, h;
			character[i]->GetLocation(x,y);
			character[i]->GetSize(w,h);
			triggers->CheckCollision(x,y,w,h);
		}

}

void CharacterManager::CheckObstacleCollisions(ObstacleManager *obstacles)
{
	for(int i=0; i<character.Top(); i++) {
		if( character[i] && (!character[i]->IsNPC()) )
		{
			float x, y, w, h;
			character[i]->GetLocation(x,y);
			character[i]->GetSize(w,h);
			bool sides[4];
			//obstacles->CheckCollision(x,y,w,h,character[i]->GetXVel(), character[i]->GetYVel(), sides);
			float lmX = character[i]->GetLastMoveX();
			float lmY = character[i]->GetLastMoveY();
			obstacles->CheckCollision(x,y,w,h,lmX,lmY, sides);
			character[i]->SetLastMoveX(lmX);
			character[i]->SetLastMoveY(lmY);
			character[i]->MoveTo(x, y);
			if(sides[0] || sides[1]) {
				character[i]->SetXVel(0.0f);
			}
			if(sides[2] || sides[3]) {
				character[i]->SetYVel(0.0f);
			}
			if(sides[2]) {
				character[i]->SetGrounded(true);
				character[i]->SetYVel(0.0f);
			}
		}
	}
}

void CharacterManager::Flush() {
	character.Flush();
}

Character* CharacterManager::GetCharacter(int ID) {
	if(character[ID])
		return character[ID];
	return NULL;
}

void Character::Draw()
{
	if(spriteID >= 0) 
	{
		GraphicsCore::DrawSprite(spriteID,(int)width,(int)height, (int)pos[0], (int)pos[1]);
	}
}

void Character::ProcessPhysics(float d_t) { 
	Vector2D newPos;
	Vector2D newVel;
	Vector2D avgVel;

	newVel = vel + acel*d_t;
	avgVel = newVel + vel;
	avgVel*=.5f;
	
	newPos = pos + avgVel*d_t;
	lastMove = newPos - pos;
	pos = newPos;
	vel = newVel;
}

void Character::Jump()  {
	if(!grounded || vel[1] != 0) return;
	grounded = false; 
	vel[1] = jumpSpeed; 
}

};
