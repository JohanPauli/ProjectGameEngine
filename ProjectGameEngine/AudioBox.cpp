#include "AudioBox.h"


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

AudioBox::~AudioBox() {
	Mix_CloseAudio();
}


bool AudioBox::init(int channels, int frequency, int chunkSize) {
	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return false;
	}


	// maybe move format support to a seperate function
	// load support for the OGG and MOD sample/music formats
	int flags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD;
	int init = Mix_Init(flags);
	if (init && flags != flags) {
		printf("Mix_Init: Failed to init required ogg, mod, and mp3 support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
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
if sound is null, returns -1
*/
int AudioBox::playSound(Sound* sound, int loops, int channel) const {
	if (sound == nullptr) {
		printf("Failed to play sound: null");
		return -1;
	}
	return Mix_PlayChannel(channel, sound->getChunk(), loops);
}


/*
returns volume ( previous, not the one being set)
if volume is -1 or unspecified, doesnt change volume, but returns current volume
if sound is null, returns -1
*/
int AudioBox::soundVolume(Sound* sound, int vol) const {
	if (sound == nullptr) {
		printf("Failed to change volume: Sound is null");
		return -1;
	}
	return Mix_VolumeChunk(sound->getChunk(), vol);
}



/*
stops the specified sound channel
*/
void AudioBox::stopSound(int channel) const {
	Mix_HaltChannel(channel);
}



void AudioBox::playMusic(Music* music, int loops) const {
	if (music == nullptr) {
		printf("Failed to play music: null");
		return;
	}
	Mix_PlayMusic(music->getMusic(), loops);
}


void AudioBox::fadeInMusic(Music* music, int ms, int loops) const {
	if (music == nullptr) {
		printf("Failed to fade-in music: null");
		return;
	}
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



Sound* AudioBox::loadSound(const std::string& path) const {
	auto chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		printf("Unable to load chunk path: %s", path.c_str());
		printf("Mix error: %s", Mix_GetError());
		return nullptr;
	}

	return new Sound(chunk);
}


Music* AudioBox::loadMusic(const std::string& path) const {
	auto mus = Mix_LoadMUS(path.c_str());
	if (mus == nullptr) {
		printf("Unable to load music path: %s", path.c_str());
		printf("Mix error: %s", Mix_GetError());
		return nullptr;
	}

	return new Music(mus);
}