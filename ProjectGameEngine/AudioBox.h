#ifndef __AUDIO_BOX_H__
#define __AUDIO_BOX_H__



#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>


// black box for Mix_Chunk
class Sound {
private:
	Mix_Chunk*	chunk;

public:
	Sound(Mix_Chunk* chunk);
	~Sound();

	Mix_Chunk*	getChunk() const;
};



// black box for Mix_Music
class Music {
private:
	Mix_Music*	music;

public:
	Music(Mix_Music* music);
	~Music();

	Mix_Music*	getMusic() const;
};



/*
wrapper for audio playing
*/
class AudioBox {
public:
	AudioBox();
	~AudioBox();

	// sound
	int  playSound(Sound* sound, int loop = 0, int channel = -1);
	int  soundVolume(Sound* sound, int vol = -1);
	void stopSound(int channel = -1);

	// music
	void playMusic(Music* music, int loop = 0);
	void fadeInMusic(Music* music, int ms, int loop = 0);
	void fadeOutMusic(int ms);
	bool isMusicPaused() const;
	void pauseMusic();
	void resumeMusic();
	int	 musicVolume(int vol = -1);
	void stopMusic();

	// general
	int  volume(int vol = -1, int channel = -1);
	void stopAllSound();
	bool changeQuality(int freq, int chunksize = 1024, int channels = 32);

	// static load functions
	static Sound*	loadSound(const std::string& path);
	static Music*	loadMusic(const std::string& path);
};

#endif