#pragma once
#include "Trigger.h"
#include "Messages.h"
#include "../GraphicsCore/GraphicsCore.h"

namespace AICore {
int TriggerManager::CreateTrigger (float x, float y, float width, float height, int spriteID, float delay, int gameSpecific)
{
	return trigger.Add(new Trigger(x, y, width, height, spriteID, delay, gameSpecific));
}

TriggerManager::TriggerManager(void):Observer(){}
TriggerManager::~TriggerManager(void){}

void TriggerManager::Update()
{
}
void TriggerManager::Draw()
{
	for(int i=0; i<trigger.Top(); i++)
		if(trigger[i] && trigger[i]->IsActive() )
			trigger[i]->Draw();
}

void TriggerManager::CheckCollision(float tx, float ty, float tw, float th)
{
	for(int i=0; i<trigger.Top(); i++)
		if( trigger[i]&&trigger[i]->IsActive() )
			trigger[i]->CheckCollision(tx,ty,tw,th);
}

void TriggerManager::Flush() {
	trigger.Flush();
}

void Trigger::CheckCollision(float tx, float ty, float tw, float th)
{
	if( tx>=x+width || tx+tw <= x || ty >= y+height || ty+th <= y ) return;
	MSGData p1, p2;
	p1.i=ID();
	p2.i=userInfo;
	Send(-1,AI_T_Trigger,AI_Q_TriggerTripped,p1,p2);
	if(delay >= 0)
		RemindMe((float)(delay+2.0),AI_T_Trigger,AI_Q_ReEnableTrigger,p1,p2);
	active=false;
}

void Trigger::Draw() {
	if(spriteID >= 0) {
		GraphicsCore::DrawSprite(spriteID,(int)width,(int)height, (int)x, (int)y);
	}
}


bool Trigger::Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2) 
{
	if(to != ID()) return false;
	switch(type) {
	case AI_T_Trigger:
		switch(qual) {
		case AI_Q_ReEnableTrigger:
		active = true;
			return true;
		}
		break;
	}
	return false;
}
};
