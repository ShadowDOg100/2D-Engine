#ifndef MAIN_CORE_H_
#define MAIN_CORE_H_

#include <windows.h>
#include <string>
#include "../AICore/Character.h"

namespace MainCore {
	void Initialize(HINSTANCE hInstance);
	void Shutdown();
	void Process();
	bool LoadGame(std::string filename);
	bool LoadLevel(std::string filename);
	bool LoadMenu(std::string filename);
	bool LoadSave(std::string filename);
	bool SaveGame(std::string saveFile);
	bool IsGameClockRunning();
	double GetGameTime();
	void PauseGameClock();
	void RunGameClock();
	void ResetGameClock();
	AICore::Character* GetCharacter(int ID);
};

#endif