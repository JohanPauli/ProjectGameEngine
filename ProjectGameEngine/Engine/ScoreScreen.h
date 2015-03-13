#pragma once

// forward declarations
class Counter;
class Renderer;
class Sprite;
class Entity;

class  ScoreScreen {
	static int highScore;
private:
	Counter* _gameScore = nullptr;
	Counter* _highScore = nullptr;

	// entities
	Entity* _scoreboard = nullptr;
	Entity* _medal = nullptr;

public:
	ScoreScreen();
	virtual ~ScoreScreen();

	virtual void setScore(int score);

	virtual void render(Renderer*);

};