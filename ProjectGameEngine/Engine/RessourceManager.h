#ifndef RESSOURCEMANAGER_H
#define RESSOURCEMANAGER_H

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Rendering.h"
#include "Sprites.h"
#include "Audio.h"



class RessourceManager
{
private:
	//TypeMap is to used in switch statement in the load function
	typedef std::map<std::string, int> TypeMap;
	//typedef is <tag, ressource>
	typedef std::map<std::string, Sprite*> SpriteMap;
	typedef std::map<std::string, Sound*> SoundMap;
	typedef std::map<std::string, Music*> MusicMap;

	TypeMap typeMap;
	SpriteMap spriteMap;
	SoundMap soundMap;
	MusicMap musicMap;

	//Maybe make renderer a singleton?
	Renderer *renderer = nullptr;
	RessourceManager(Renderer *);
	RessourceManager(RessourceManager const&) = delete;
	void operator=(RessourceManager const&)		= delete;

public:
	~RessourceManager();
	bool load(std::string filename);

	//Template function specified in cpp file
	template<class T> 
	T getByTag(std::string tag);

	//return singleton
	static RessourceManager& getInstance(Renderer *);

};

#endif