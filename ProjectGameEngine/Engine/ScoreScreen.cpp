#include "ScoreScreen.h"

#include "Entity.h"
#include "Counter.h"
#include "EntityGenerators.h"
#include "Rendering.h"

int ScoreScreen::highScore = 0;


ScoreScreen::ScoreScreen() {}

ScoreScreen::~ScoreScreen() {
	delete _gameScore;
	delete _highScore;
	delete _scoreboard;
	delete _medal;
}


void ScoreScreen::setScore(int score) {

	double scale = 4.0;
	int sbX = 100;
	int sbY = 100;
	int medalX = sbX + (13*scale);
	int medalY = sbY + (50 * scale);

	double scoreScale = 1.8;
	int scoreX = sbX + (90 * scale);
	int scoreY = sbY + (45 * scale);
	int hscoreY = sbY + (65 * scale);

	_scoreboard = EntityGenerator::getInstance().createScoreboard(sbX, sbY, scale);

	// init Counters
	_gameScore = new Counter(scoreX, scoreY, scoreScale, score);
	_highScore = new Counter(scoreX, hscoreY, scoreScale, highScore);

	// set medal
	if (score <= 1)
		_medal = nullptr;
	else if (score < 5)
		_medal = EntityGenerator::getInstance().createMedalBronze(medalX, medalY, scale);
	else if (score < 10)
		_medal = EntityGenerator::getInstance().createMedalSilver(medalX, medalY, scale);
	else if (score < 20)
		_medal = EntityGenerator::getInstance().createMedalGold(medalX, medalY, scale);
	else
		_medal = EntityGenerator::getInstance().createMedalPlatinum(medalX, medalY, scale);

	// check for new high score
	if (score > highScore)
		highScore = score;
}


void ScoreScreen::render(Renderer* renderer) {
	if (_scoreboard != nullptr) {
		_scoreboard->render(renderer);
		_gameScore->render(renderer);
		if (_highScore != nullptr)
			_highScore->render(renderer);
		if (_medal != nullptr)
			_medal->render(renderer);
	}
}