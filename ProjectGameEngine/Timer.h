#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>

class Timer {
private:
	long long	gameTime = 0;
	long long	previous = 0;
	long long	current =  0;
	long long	elapsed =  0;
	long long	updateLag = 0;
	bool		paused = true;


public:
	// start the timer
	void start() {
		if (paused) {
			paused = false;

			current = realTime();
			previous = current;
		}
	}

	// update the timer
	void update() {
		if (!paused) {
			paused = false;
			current = realTime();
			elapsed = current - previous;
			previous = current;
			updateLag += elapsed;
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

	// milliseconds since last update
	long long getUpdateLag() const {
		return updateLag;
	}

	// ms to trim off of updateLag, use after each update
	void trimUpdateLag(int ms) {
		updateLag -= ms;
	}

	// milliseconds since epoch 
	static long long realTime() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch())
			.count();
	}
};




#endif