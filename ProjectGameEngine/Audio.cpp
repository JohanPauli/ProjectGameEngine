#include "Audio.h"

#include "Debug.h"

// ---- SoundEffect ----

Sound::Sound(Mix_Chunk* chunk)
	: chunk(chunk)
{}

Sound::~Sound() { Mix_FreeChunk(chunk); }


void Sound::setVolume(int vol) {
	chunk->volume = vol;
}


// ---- Music ----

Music::Music(Mix_Music* music)
	: music(music)
{}

Music::~Music() { Mix_FreeMusic(music); }




// ---- Audio ----

Audio::Audio() {}

Audio::~Audio() {
	Mix_CloseAudio();
}

Audio& Audio::get() {
	static Audio instance;
	return instance;
}


bool Audio::init(int channels, int frequency, int chunkSize) {
	if (SDL_Init(SDL_INIT_AUDIO < 0)) {
		LOG("SDL_Init failed on SDL_INIT_AUDIO: " << SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) == -1) {
		LOG(Mix_GetError());
		return false;
	}


	// maybe move format support to a seperate function
	// load support for the OGG and MOD sample/music formats
	int flags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD;
	int init = Mix_Init(flags);
	if (init && flags != flags) {
		LOG(Mix_GetError());
		return false;
	}
	initialized = true;
	return true;
}

bool Audio::isInitialized() const {
	return initialized;
}


/*
returns channel that sound is played on
*/
int Audio::playSound(Sound* sound, int loops, int channel) const {
	return Mix_PlayChannel(channel, sound->chunk, loops);
}


/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
*/
int Audio::soundVolume(Sound* sound, int vol) const {
	return Mix_VolumeChunk(sound->chunk, vol);
}



/*
stops the specified sound channel
*/
void Audio::stopSound(int channel) const {
	Mix_HaltChannel(channel);
}



void Audio::playMusic(Music* music, int loops) const {
	Mix_PlayMusic(music->music, loops);
}


void Audio::fadeInMusic(Music* music, int ms, int loops) const {
	Mix_FadeInMusic(music->music, loops, ms);
}

void Audio::fadeOutMusic(int ms) const {
	Mix_FadeOutMusic(ms);
}

void Audio::pauseMusic() const {
	if (Mix_PausedMusic() == 0)
		Mix_PauseMusic();
}

bool Audio::isMusicPaused() const {
	/* Mix_PausedMusic returns 1 if music is paused and 0 otherwise.
	Can you simply
	return Mix_PausedMusic(); ?
	*/
	return Mix_PausedMusic() == 1;
}

void Audio::resumeMusic() const {
	Mix_ResumeMusic();
}

/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
*/
int Audio::musicVolume(int vol) const {
	return Mix_VolumeMusic(vol);
}



void Audio::stopMusic() const {
	Mix_HaltMusic();
}


/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
if channel is -1 or unspecified, set volume for all channels
*/
int Audio::volume(int vol, int channel) const {
	return Mix_Volume(channel, vol);
}


void Audio::allocateChannels(int n) const {
	Mix_AllocateChannels(n);
}


bool Audio::isChannelAvailable(int channel) const {
	if (Mix_Playing(channel))
		return false;
	if (Mix_Paused(channel))
		return false;
	return true;
}


void Audio::stopAllSound() const {
	stopMusic();
	stopSound();
}



Sound* Audio::loadSound(const std::string& path) const {
	auto chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		LOG("path: " << path.c_str() << ", error: " << Mix_GetError());
		return nullptr;
	}

	return new Sound(chunk);
}


Music* Audio::loadMusic(const std::string& path) const {
	auto mus = Mix_LoadMUS(path.c_str());
	if (mus == nullptr) {
		LOG("path: " << path.c_str() << ", error: " << Mix_GetError());
		return nullptr;
	}

	return new Music(mus);
}