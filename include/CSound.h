/*
 * CSound.h
 *
 *  Created on: 17 sept. 2021
 *      Author: salim
 */
#include <SDL2/SDL_mixer.h>

#include <parameters.h>

#define SOUND_DEFAULT_MUSIC_VOLUME		((int)(128/2))

class CSound {
private:
	Mix_Music*		m_pMusic;

	Mix_Chunk*		m_pSoundCollect;
	Mix_Chunk*		m_pSoundPoison;
	Mix_Chunk*		m_pSoundPause;

public:
	CSound();
	~CSound();
	void MusicPause();
	void MusicPlay();
	void CollectPlay();
	void PoisonPlay();
	void PausePlay();
};
