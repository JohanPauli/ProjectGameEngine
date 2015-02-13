#include "Timer.h"



// ---- Timer ----

// milliseconds since epoch 
long long Timer::getRealTime() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch())
		.count();
}

// amount of time last update took
long long Timer::getElapsed() const {
	return elapsed;
}


void Timer::start() {
	if (paused) {
		paused = false;

		current = getRealTime();
		previous = current;
	}
}



void Timer::update() {
	if (!paused) {
		paused = false;
		current = getRealTime();
		elapsed = current - previous;
		previous = current;
		gameTime += elapsed;

	}
}

// pause the timer
void Timer::pause() {
	if (!paused) {
		update();
		paused = true;
	}
}

// resume the timer
void Timer::resume() {
	start();
}

// time since game started
long long Timer::getGameTime() const {
	return gameTime;
}




// ---- UpdateTimer ----

UpdateTimer::UpdateTimer(Timer* timer, int timePerUpdate) 
	: timer(timer), timePerUpdate(timePerUpdate) {}


void UpdateTimer::update() {
	long long udTime = timer->getGameTime();
	if (lastUpdateTime != udTime) {
		updateLag += timer->getElapsed();
		lastUpdateTime = timer->getGameTime();
	}

}


// milliseconds since last update
long long UpdateTimer::getUpdateLag() {
	update();
	return updateLag;
}


bool UpdateTimer::isTimeToUpdate() {
	update();
	return timePerUpdate <= updateLag;
}

// ms to trim off of updateLag, use after each update
void UpdateTimer::updated() {
	updateLag -= timePerUpdate;
}

