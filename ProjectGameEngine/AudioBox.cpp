#include "AudioBox.h"

#include "Debug.h"

// ---- SoundEffect ----

Sound::Sound(Mix_Chunk* chunk)
	: chunk(chunk)
{}

Sound::~Sound() { Mix_FreeChunk(chunk); }


Mix_Chunk* Sound::getChunk() const	{
	return chunk;
}

void Sound::setVolume(int vol) {
	chunk->volume = vol;
}


// ---- Music ----

Music::Music(Mix_Music* music)
	: music(music)
{}

Music::~Music() { Mix_FreeMusic(music); }



Mix_Music* Music::getMusic() const	{
	return music;
}



// ---- AudioBox ----

AudioBox::AudioBox() {}

AudioBox::~AudioBox() {
	Mix_CloseAudio();
}

AudioBox& AudioBox::get() {
	static AudioBox instance;
	return instance;
}


bool AudioBox::init(int channels, int frequency, int chunkSize) {
	assert(channels > 0); assert(frequency > 0); assert(chunkSize > 0);
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

bool AudioBox::isInitialized() const {
	return initialized;
}


/*
returns channel that sound is played on
*/
int AudioBox::playSound(Sound* sound, int loops, int channel) const {
	assert(sound != nullptr);
	return Mix_PlayChannel(channel, sound->getChunk(), loops);
}


/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
*/
int AudioBox::soundVolume(Sound* sound, int vol) const {
	assert(sound != nullptr);
	return Mix_VolumeChunk(sound->getChunk(), vol);
}



/*
stops the specified sound channel
*/
void AudioBox::stopSound(int channel) const {
	Mix_HaltChannel(channel);
}



void AudioBox::playMusic(Music* music, int loops) const {
	assert(music != nullptr);
	Mix_PlayMusic(music->getMusic(), loops);
}


void AudioBox::fadeInMusic(Music* music, int ms, int loops) const {
	assert(music != nullptr);
	Mix_FadeInMusic(music->getMusic(), loops, ms);
}

void AudioBox::fadeOutMusic(int ms) const {
	Mix_FadeOutMusic(ms);
}

void AudioBox::pauseMusic() const {
	if (Mix_PausedMusic() == 0)
		Mix_PauseMusic();
}

bool AudioBox::isMusicPaused() const {
	/* Mix_PausedMusic returns 1 if music is paused and 0 otherwise.
	Can you simply
	return Mix_PausedMusic(); ?
	*/
	return Mix_PausedMusic() == 1;
}

void AudioBox::resumeMusic() const {
	Mix_ResumeMusic();
}

/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
*/
int AudioBox::musicVolume(int vol) const {
	return Mix_VolumeMusic(vol);
}



void AudioBox::stopMusic() const {
	Mix_HaltMusic();
}


/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
if channel is -1 or unspecified, set volume for all channels
*/
int AudioBox::volume(int vol, int channel) const {
	return Mix_Volume(channel, vol);
}


void AudioBox::allocateChannels(int n) const {
	Mix_AllocateChannels(n);
}


bool AudioBox::isChannelAvailable(int channel) const {
	if (Mix_Playing(channel))
		return false;
	if (Mix_Paused(channel))
		return false;
	return true;
}


void AudioBox::stopAllSound() const {
	stopMusic();
	stopSound();
}



Sound* AudioBox::loadSound(const std::string& path) {
	assert(path != "");
	auto chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		LOG("path: " << path.c_str() << ", error: " << Mix_GetError());
		return nullptr;
	}

	return new Sound(chunk);
}


Music* AudioBox::loadMusic(const std::string& path) {
	assert(path != "");
	auto mus = Mix_LoadMUS(path.c_str());
	if (mus == nullptr) {
		LOG("path: " << path.c_str() << ", error: " << Mix_GetError());
		return nullptr;
	}

	return new Music(mus);
}