#pragma once


#include <chrono>

/*
	General timer for keeping track of game time
*/
class Timer {
private:
	friend class UpdateTimer;

	long long	gameTime = 0;
	long long	previous = 0;
	long long	current =  0;
	long long	elapsed =  0;
	bool		paused = true;

private:
	// milliseconds since epoch 
	long long getRealTime() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch())
			.count();
	}

public:

	// start the timer
	void start() {
		if (paused) {
			paused = false;

			current = getRealTime();
			previous = current;
		}
	}

	// update the timer
	void update() {
		if (!paused) {
			paused = false;
			current = getRealTime();
			elapsed = current - previous;
			previous = current;
			gameTime += elapsed;

		}
	}

	// pause the timer
	void pause() {
		if (!paused) {
			update();
			paused = true;
		}
	}

	// resume the timer
	void resume() {
		start();
	}

	// time since game started
	long long getGameTime() const {
		return gameTime;
	}
};


/*
	Timer for keeping track of when to update things
*/
class UpdateTimer {
private:
	Timer*		timer;
	int			timePerUpdate;		// update frequency in milliseconds
	long long	updateLag = 0;		// how far behind the update is in milliseconds
	long long	lastUpdateTime = -1;// last time this class invoked its own update()

private:
	// attempt to update the updateLag variable
	void update() {
		long long udTime = timer->getGameTime();
		if (lastUpdateTime != udTime) {
			updateLag += timer->elapsed;
			lastUpdateTime = timer->getGameTime();
		}

	}

public:
	UpdateTimer(Timer*timer, int timePerUpdate)
		: timer(timer), timePerUpdate(timePerUpdate) {}

	// milliseconds since last update
	long long getUpdateLag() {
		update();
		return updateLag;
	}

	// check if timePerUpdate exceeds updateLag
	bool isTimeToUpdate() {
		update();
		return timePerUpdate <= updateLag;
	}

	// trim timePerUpdate off of updateLag. use after each update
	void updated() {
		updateLag -= timePerUpdate;
	}

	// reset updateLag
	void catchUp() {
		updateLag = 0;
	}
};

