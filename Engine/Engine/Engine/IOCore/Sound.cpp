#include "Sound.h"
#include <iostream>

// Initialize the sound singleton
Sound* Sound::m_SoundInstance = 0;

Sound* Sound::GetSoundInstance(void)
{
	// Create the singleton if it doesn't exist
	if(!m_SoundInstance)
		m_SoundInstance = new Sound;

	// Return the singleton
	return m_SoundInstance;
}

void Sound::DeleteInstance(void)
{
	// If instance exist, delete it!
	if(m_SoundInstance)
	{
		delete m_SoundInstance;
		m_SoundInstance = NULL;
	}
}

Sound::Sound(void)
{
}

Sound::~Sound(void)
{
}

//void Sound::InitSounds(void)
//{
//	// Initialize the sound middleware
//	FMOD::System_Create( &system );	// Create the main system object.
//
//	system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD
//
//	// Create Game Music
//	system->createStream("", FMOD_LOOP_NORMAL, 0, &levelOneMusic );
//
//
//	// Create Collision Sound Effects
//	system->createSound("", FMOD_DEFAULT, 0, &collItem );
//
//	// Create Input Sound Effects
//	system->createSound("", FMOD_DEFAULT, 0, &menuBtnClick );
//	system->createSound("", FMOD_DEFAULT, 0, &selected );
//
//	// Create Animation Sound Effects
//	system->createSound("", FMOD_DEFAULT, 0, &playerB2W );
//	system->createSound("", FMOD_DEFAULT, 0, &playerW2B );
//}
//
//// Music methods
//void Sound::MenuScreenMusic()
//{
//	channel->stop();
//	// Play the menu sound 
//	system->playSound( FMOD_CHANNEL_FREE, menuScreenMusic, false, &channel );
//}
//void Sound::LevelOneMusic()
//{
//	channel->stop();
//	system->playSound( FMOD_CHANNEL_FREE, levelOneMusic, false, &channel );
//}
//
//// Collision SFX methods
//
//void Sound::ItemCollisionSFX()
//{
//	system->playSound( FMOD_CHANNEL_FREE, collItem, false, 0 );
//}
//
//// Input SFX methods
//void Sound::MenuButtonClickSFX()
//{
//	system->playSound( FMOD_CHANNEL_FREE, menuBtnClick, false, 0 );
//}
//void Sound::selectedSFX()
//{
//	system->playSound( FMOD_CHANNEL_FREE, selected, false, 0 );
//}
//
//// Update Sound
//void Sound::Update()
//{
//	system->update();
//}