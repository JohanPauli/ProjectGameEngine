#include "RessourceManager.h"

RessourceManager::RessourceManager()
{
	//define typemap integers
	typeMap["SPRITE"] = 1;
	typeMap["SOUND"] = 2;
	typeMap["MUSIC"] = 3;
}
//
//
RessourceManager::~RessourceManager()
{
	/*auto spriteIt = spriteMap.begin();
	while (spriteIt != spriteMap.end())
	{
		Sprite *sprite = spriteIt->second;
		delete sprite;
		sprite = nullptr;
		spriteIt++;
	}
	auto soundIt = soundMap.begin();
	while (soundIt != soundMap.end())
	{
		Sound *sound = soundIt->second;
		delete sound;
		sound = nullptr;
		soundIt++;
	}
	auto musicIt = musicMap.begin();
	while (musicIt != musicMap.end())
	{
		Music *music = musicIt->second;
		delete music;
		music = nullptr;
		musicIt++;
	}*/
	spriteMap.clear();
	soundMap.clear();
	musicMap.clear();
}

bool RessourceManager::load(std::string filename, Renderer *renderer)
{
	std::ifstream inFile;
	std::stringstream ss;
	inFile.open(filename, std::ios::in);
	std::string line;
	if (!inFile.is_open())
		return false;

	while (!inFile.eof())
	{
		getline(inFile, line, '|');
		if (!line.empty() && (*line.begin() == '\n' || *line.begin() == '\r'))
			line.erase(line.begin(), line.begin() + 1);

		//This might not be the best way to do this
		switch (typeMap.find(line)->second)
		{
			//case SPRITE
		case 1:
		{
			std::string tag = "", spriteloc = "";
			getline(inFile, tag, '|');
			getline(inFile, spriteloc);
			Sprite *loadSprite = renderer->loadSprite(spriteloc);
			if (!spriteloc.empty())
			if (loadSprite != nullptr && !tag.empty())
				spriteMap[tag] = loadSprite;
			//else Log error

			break;
		}
		//case SOUND
		case 2:
		{
			std::string tag = "", soundloc = "";
			Audio &audio = Audio::get();
			getline(inFile, tag, '|');
			getline(inFile, soundloc);

			Sound *loadSound = audio.loadSound(soundloc);
			if (loadSound != nullptr && !tag.empty())
				soundMap[tag] = loadSound;
			//else log error
			
			break;
		}
		//case MUSIC
		case 3:
		{
			std::string tag = "", musicloc = "";
			Audio &audio = Audio::get();
			getline(inFile, tag, '|');
			getline(inFile, musicloc);

			Music *loadMusic = audio.loadMusic(musicloc);
			if (loadMusic != nullptr && !tag.empty())
				musicMap[tag] = loadMusic;
			//else log error

			break;
		}
		default:
			break;
		}

		
	}
	inFile.close();
	return true;

}

template<>
Sprite* RessourceManager::getByTag<Sprite*>(std::string tag)
{
	auto sprite = spriteMap.find(tag);
	if (sprite != spriteMap.end())
		return sprite->second;
	return nullptr;
}

template<>
Sound* RessourceManager::getByTag<Sound*>(std::string tag)
{
	auto sound = soundMap.find(tag);
	if (sound != soundMap.end())
		return sound->second;
	return nullptr;
}

template<>
Music* RessourceManager::getByTag<Music*>(std::string tag)
{
	auto music = musicMap.find(tag);
	if (music != musicMap.end())
		return music->second;
	return nullptr;
}


RessourceManager& RessourceManager::getInstance()
{
	static RessourceManager instance;
	return instance;
}