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
	const static int VOLUME_MAX = MIX_MAX_VOLUME;

public:
	Sound(Mix_Chunk* chunk);
	~Sound();

	Mix_Chunk*	getChunk() const;
	void setVolume(int vol);
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
	encapsulates SDL_mixer's audio functionality
	singleton pattern with lazy initialization
*/
class AudioBox {
private:
	bool initialized = false;
private:
	AudioBox();
	AudioBox(const AudioBox&)			= delete;
	AudioBox operator=(const AudioBox&) = delete;
public:
	const static int VOLUME_MAX = MIX_MAX_VOLUME;
	const static Uint8 DISTANCE_MAX = 255;


public:
	~AudioBox();

	static AudioBox& get();


	// initialise
	bool init(int channels, int freq, int chunkSize);
	bool isInitialized() const;

	// sound
	int  playSound(Sound* sound, int loop = 0, int channel = -1) const;
	int  soundVolume(Sound* sound, int vol = -1) const;
	void stopSound(int channel = -1) const;

	// music
	void playMusic(Music* music, int loop = 0) const;
	void fadeInMusic(Music* music, int ms, int loop = 0) const;
	void fadeOutMusic(int ms) const;
	bool isMusicPaused() const;
	void pauseMusic() const;
	void resumeMusic() const;
	int	 musicVolume(int vol = -1) const;
	void stopMusic() const;

	// general
	int  volume(int vol = -1, int channel = -1) const;
	void allocateChannels(int n) const;
	bool isChannelAvailable(int channel) const;
	void stopAllSound() const;

	// static load functions
	static Sound*	loadSound(const std::string& path);
	static Music*	loadMusic(const std::string& path);
};

#endif