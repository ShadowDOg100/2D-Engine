#pragma once

#include "d3d9.h"
#include "d3dx9.h"
#include <stdio.h>
#include <sstream>
#include <string>

// Add pragma comments so we do not have to change
//  the project dependencies
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

//// FMOD library and includes (sound middleware)
//#include "fmod.h"
//#include "fmod.hpp"
//#include "fmod_codec.h"
//#include "fmod_dsp.h"
//#include "fmod_errors.h"
//#include "fmod_memoryinfo.h"
//#include "fmod_output.h"
//#pragma comment(lib, "fmodex_vc.lib")
//// End FMOD library and includes
//
//class Sound
//{
//private:
//
//	// Singleton instance
//	static Sound*	m_SoundInstance;
//
//	// Set up the FMOD sound middleware references
//	FMOD::System	*system;
//	FMOD_RESULT		fModResult;
//	FMOD::Sound		*sound[23];
//
//	// Channels
//	FMOD::Channel	*channel;
//
//	// Game Music
//	FMOD::Sound		*menuScreenMusic;
//	FMOD::Sound		*levelOneMusic;
//
//	// Collision Sound Effects
//	FMOD::Sound		*collItem;
//
//	// Input Sound Effects
//	FMOD::Sound		*menuBtnClick;
//	FMOD::Sound		*selected;
//
//	// Animation Sound Effects
//	FMOD::Sound		*playerB2W;
//	FMOD::Sound		*playerW2B;
//
//	// End FMOD sound middleware references

public:
	// constructor
	Sound(void);

	// destructor
	~Sound(void);

	// Singleton functions
	static Sound* GetSoundInstance(void);
	void DeleteInstance(void);

	// Initialize all the sounds
	void InitSounds(void);

	// Sound methods
	void Update();

	// Music methods
	void MenuScreenMusic();
	void LevelOneMusic();

	// Collision SFX methods
	void ItemCollisionSFX();

	// Input SFX methods
	void MenuButtonClickSFX();
	void selectedSFX();

};
