#include "MainCore.h"
#include "../GraphicsCore/GraphicsCore.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "MainCoreMessages.h"
#include "MainCoreMessaging.h"
#include "../AICore/AICORE.h"
#include "../GraphicsCore/GraphicsCore.h"
#include "../IOCore/IOCore.h"

#define FILE_PATH	"..\\Engine\\"

using namespace std;

namespace MainCore {
	int FPS = 30;
	int PPS = 24;
	double gameStartTime=0.0;
	double gameTime = 0.0;
	bool gameClockRunning = false;

	void UpdateGameClock()
	{
		if( gameClockRunning )
		{
			gameTime = GetTime() - gameStartTime;
		}
	}
	void ResetGameClock()
	{
		gameClockRunning = false;
		gameTime = 0.0;
		gameStartTime = GetTime();
	}
	void PauseGameClock()
	{
		gameClockRunning = false;
	}
	void RunGameClock()
	{
		gameStartTime = GetTime() - gameTime;
		gameClockRunning = true;
	}
	bool IsGameClockRunning(){ return gameClockRunning; }

	double GetGameTime()
	{
		return gameTime;
	}

	void Initialize(HINSTANCE hInstance) {
		MCMessage = new MainCoreMessaging();

		GraphicsCore::Initialize(hInstance);
		IOCore::Initialize();
		AICore::Initialize();
	
		game = new BaseGame();
		game->Initialize();

		MCMessage->RemindMe((float)(1.0/FPS), MC_T_REMINDME, MC_Q_DRAW, 0, 0);
		MCMessage->RemindMe((float)(1.0/PPS), MC_T_REMINDME, MC_Q_PROCESS, 0, 0);
	}

	void Process() {
		UpdateGameClock();
		DISPATCH.Process();
	}

	bool MainCoreMessaging::Notify(int to, int from, int type, int qual, MSGData p1, MSGData p2) {
		if(type == MC_T_REMINDME) {
			if(qual == MC_Q_DRAW) {
				MCMessage->RemindMe((float)(1.0/FPS), MC_T_REMINDME, MC_Q_DRAW, 0, 0);

				GraphicsCore::StartDraw();
				GraphicsCore::UseCamera(true);
				AICore::Draw();
				GraphicsCore::UseCamera(false);
				IOCore::Draw();
				game->Draw();
				GraphicsCore::FinishDraw();

				return true;
			} else if(qual == MC_Q_PROCESS) {
				MCMessage->RemindMe((float)(1.0/PPS), MC_T_REMINDME, MC_Q_PROCESS, 0, 0);
				
				AICore::Process();
				IOCore::Process();
				game->Process();
				return true;
			}
		}
		return false;
	}

	void Shutdown() {
		AICore::Shutdown();
		IOCore::Shutdown();
		GraphicsCore::Shutdown();

		//delete game;
		delete MCMessage;
	}

	bool LoadGame(std::string filename) {
		std::ifstream fileStream;
		std::string filePath = FILE_PATH;
		filePath = filePath + "LevelInfo\\" + filename + ".txt";
		fileStream.open(filePath);
		if(!fileStream.good())
			return false;

		while(!fileStream.eof())
		{
			char buf[512];
			fileStream.getline(buf, 512);

			int n = 0;

			const char* token[20];
			char *context=NULL;
		
			token[0] = strtok_s(buf, ",",&context);
			if(token[0]) {
				for(n = 1; n < 20; n++) {
					token[n] = strtok_s(0, ",", &context);
					if(!token[n]) break;
				}
			}

			string token_str[20];
			for(int i = 0; i < 20; i++) {
				if(!token[i]) break;
				token_str[i] = std::string(token[i]);
			}

			if(token_str[0] == "start") {
				std::string level = token[2];
				if(token_str[1] == "menu") {
					MainCore::LoadMenu(level);
				}
				else if(token_str[1] == "level") {
					MainCore::LoadLevel(level);
				}
			}

			if(token_str[0] == "fps") {
				FPS = atoi(token[1]);
			}

			if(token_str[0] == "pps") {
				PPS = atoi(token[1]);
			}
		}

		return true;
	}

	struct SpriteAssociation {
		std::string name;
		int id;
	};

	bool LoadLevel(std::string filename) {
		std::ifstream fileStream;
		std::string filePath = FILE_PATH;
		filePath = filePath + "LevelInfo\\" + filename + ".txt";
		fileStream.open(filePath);
		if(!fileStream.good())
			return false;

		std::string levelName;
		int width, height;
		GenArray<SpriteAssociation> spriteAssociations;

		AICore::ClearLevel();
		IOCore::ClearLevel();
		GraphicsCore::ClearSprites();

		while(!fileStream.eof())
		{
			char buf[512];
			fileStream.getline(buf, 512);

			int n = 0;

			const char* token[20];
			char *context = NULL;
			token[0] = strtok_s(buf, ",",&context);
			if(token[0]) {
				for(n = 1; n < 20; n++) {
					token[n] = strtok_s(0, ",", &context);
					if(!token[n]) break;
					}
			} else {
				continue;
			}

			string token_str[20];
			for(int i = 0; i < 20; i++) {
				if(!token[i]) break;
				token_str[i] = std::string(token[i]);
			}

			if(token_str[0] == "//")
				continue;

			if(token_str[0] == "levelinfo") {
				levelName = token[1];
				width = atoi(token[2]);
				height = atoi(token[3]);
				continue;
			}

			if(token_str[0] == "gamemessageF") {
				MSGData p1, p2;
				int type, qual;
				type = atoi(token[1]);
				qual = atoi(token[2]);
				p1.f = (float)atof(token[3]);
				p2.f = (float)atof(token[4]);
				MCMessage->Send(game->ID(), type, qual, p1, p2, 0.0f);
			}

			if(token_str[0] == "gamemessageI") {
				MSGData p1, p2;
				int type, qual;
				type = atoi(token[1]);
				qual = atoi(token[2]);
				p1.i = atoi(token[3]);
				p2.i = atoi(token[4]);
				MCMessage->Send(game->ID(), type, qual, p1, p2, 0.0f);
			}

			if( token_str[0]=="gamemessageSprite")
			{
				MSGData p1, p2;
				int type, qual;
				type = atoi(token[1]);
				qual = atoi(token[2]);
				p1.i = atoi(token[3]);
				p2.i = -1;
				std::string spriteName = token[4];
				for(int i = 0; i < spriteAssociations.Top(); i++) {
					if(spriteAssociations[i]->name == spriteName) {
						p2.i = spriteAssociations[i]->id;
					}
				}
				if( p2.i >= 0 )
					MCMessage->Send(game->ID(), type, qual, p1, p2, 0.0f);
			}

			if(token_str[0] == "loadsprite") {
				SpriteAssociation *s = new SpriteAssociation();
				s->name = token[1];
				std::string fileName = FILE_PATH;
				fileName = fileName + "sprites\\" + token[2];
				bool looping = (token_str[8] == "true") ? true : false;
				s->id = GraphicsCore::AddSprite(fileName.c_str(), atoi(token[5]), atoi(token[6]), atoi(token[3]), atoi(token[4]), atoi(token[7]), looping);
				spriteAssociations.Add(s);
			}

			if(token_str[0] == "background") {
				int id = -1;
				std::string spriteName = token[1];
				if(spriteName != "none") {
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id = spriteAssociations[i]->id;
						}
					}
					if(id == -1)
						return false;
				}
				GraphicsCore::SetBackground(id, width, height);
			}

			if(token_str[0] == "player") {
				bool npc = (token_str[1] == "player") ? false : true;
				int id = -1;
				std::string spriteName = token[6];
				for(int i = 0; i < spriteAssociations.Top(); i++) {
					if(id != -1)
						continue;
					if(!spriteAssociations[i])
						continue;
					if(spriteAssociations[i]->name == spriteName) {
						id = spriteAssociations[i]->id;
					}
				}
				if(id == -1)
					return false;
				AICore::CreatePlayerCharacter(npc, id, 
											(float)atof(token[2]), 
											(float)atof(token[3]), 
											(float)atof(token[4]), 
											(float)atof(token[5]), 
											atoi(token[7]));
				GraphicsCore::MoveCameraTo(atoi(token[2]), atoi(token[3]));
			}

			if(token_str[0] == "obsticle") {
				bool solid = (token_str[6] == "true") ? true : false;
				int id = -1;
				std::string spriteName = token[5];
				if(spriteName != "none") {
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id = spriteAssociations[i]->id;
						}
					}
					if(id == -1)
						return false;
				}
				AICore::CreateObstacle(id, 
										(float)atof(token[1]), 
										(float)atof(token[2]), 
										(float)atof(token[3]), 
										(float)atof(token[4]), 
										solid, 
										atoi(token[7]));
			}

			if(token_str[0] == "trigger") {
				int id = -1;
				std::string spriteName = token[5];
				if(spriteName != "none") {
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id = spriteAssociations[i]->id;
						}
					}
					if(id == -1)
						return false;
				}
				AICore::CreateTrigger(id, 
									(float)atof(token[1]), 
									(float)atof(token[2]), 
									(float)atof(token[3]), 
									(float)atof(token[4]), 
									(float)atof(token[6]), 
									atoi(token[7]));
			}

			if(token_str[0] == "loadmenu") {
				LoadMenu(token_str[1]);
			}
		}
		
		game->SetLevel(levelName);

		fileStream.close();
		return true;
	}

	bool LoadMenu(std::string filename) {
		std::ifstream fileStream;
		std::string filePath = FILE_PATH;
		filePath = filePath + "LevelInfo\\" + filename + ".txt";
		fileStream.open(filePath);
		if(!fileStream.good())
			return false;

		std::string menuName;
		GenArray<SpriteAssociation> spriteAssociations;
		int menuId = -1;

		while(!fileStream.eof())
		{
			char buf[512];
			fileStream.getline(buf, 512);

			int n = 0;

			const char* token[20];
			char *context=NULL;
		
			token[0] = strtok_s(buf, ",", &context);
			if(token[0]) {
				for(n = 1; n < 20; n++) {
					token[n] = strtok_s(0, ",", &context);
					if(!token[n]) break;
				}
			} else {
				continue;
			}

			string token_str[20];
			for(int i = 0; i < 20; i++) {
				if(!token[i]) break;
				token_str[i] = std::string(token[i]);
			}

			if(token_str[0] == "//")
				continue;

			
			if(token_str[0] == "name") {
				menuName = token[1];
				continue;
			}

			if(token_str[0] == "loadsprite") {
				SpriteAssociation *s = new SpriteAssociation();
				s->name = token[1];
				std::string fileName = FILE_PATH;
				fileName = fileName + "sprites\\" + token[2];
				bool looping = (token[8] == "true") ? true : false;
				s->id = GraphicsCore::AddSprite(fileName.c_str(), atoi(token[5]), atoi(token[6]), atoi(token[3]), atoi(token[4]), atoi(token[7]), looping);
				spriteAssociations.Add(s);
			}

			if(token_str[0] == "menudata") {
				int id = -1;
				std::string spriteName = token[5];
				if(spriteName != "none") {
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id = spriteAssociations[i]->id;
						}
					}
					if(id == -1)
						return false;
				}
				menuId = IOCore::CreateMenu(atoi(token[1]), atoi(token[2]), atoi(token[3]), atoi(token[4]), id, atoi(token[6]));
			}

			if(token_str[0] == "button") {
				if(menuId == -1) 
					continue;

				int id[3];
				id[0] = -1;
				id[1] = -1;
				id[2] = -1;
				for(int j = 0; j < 3; j++) {
					std::string spriteName = token[6+j];
					if(spriteName == "none" || spriteName == "-1")
						continue;
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id[j] != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id[j] = spriteAssociations[i]->id;
						}
					}
					if(id[j] == -1)
						return false;
				}

				IOCore::CreateButton(menuId, atoi(token[1]), atoi(token[2]), atoi(token[3]), atoi(token[4]), token[5], id[0], id[1], id[2], atoi(token[9]));
			}

			if(token_str[0] == "bar") {
				if(menuId == -1)
					continue;
				int id[2];
				id[0] = -1;
				id[1] = -1;
				for(int j = 0; j < 2; j++) {
					std::string spriteName = token[6+j];
					if(spriteName == "none" || spriteName == "-1")
						continue;
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id[j] != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id[j] = spriteAssociations[i]->id;
						}
					}
					if(id[j] == -1)
						return false;
				}
				std::string text = token[5];
				if(text == "none")
					text = "";
				IOCore::CreateBar(menuId, atoi(token[1]),atoi(token[2]),atoi(token[3]),atoi(token[4]),text,id[0],id[1],atoi(token[8]));
			}

			if(token_str[0] == "timer") {
				if(menuId == -1)
					continue;
				int id = -1;
				std::string spriteName = token[6];
				if(!(spriteName == "none" || spriteName == "-1"))
				{
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id = spriteAssociations[i]->id;
						}
					}
					if(id == -1)
						return false;
				}
				std::string text = token[5];
				if(text == "none")
					text = "";
				IOCore::CreateTimer(menuId, atoi(token[1]),atoi(token[2]),atoi(token[3]),atoi(token[4]),text,id,atoi(token[7]));
			}

			if(token_str[0] == "text") {
				if(menuId == -1)
					continue;
				int id = -1;
				std::string spriteName = token[6];
				if(!(spriteName == "none" || spriteName == "-1"))
				{
					for(int i = 0; i < spriteAssociations.Top(); i++) {
						if(id != -1)
							continue;
						if(!spriteAssociations[i])
							continue;
						if(spriteAssociations[i]->name == spriteName) {
							id = spriteAssociations[i]->id;
						}
					}
					if(id == -1)
						return false;
				}
				std::string text = token[5];
				if(text == "none")
					text = "";
				IOCore::CreateText(menuId, atoi(token[1]),atoi(token[2]),atoi(token[3]),atoi(token[4]),text,id,atoi(token[7]));
			}
		}

		return true;
	}

	AICore::Character* GetCharacter(int ID) {
		return AICore::GetCharacter(ID);
	}
};