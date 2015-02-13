#ifndef __TIMER_H__
#define __TIMER_H__

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
	long long getRealTime() const;
	long long getElapsed() const;	// time elapsed for last loop

public:

	// start the timer
	void start();

	// update the timer
	void update();

	// pause the timer
	void pause();

	// resume the timer
	void resume();

	// time since game started
	long long getGameTime() const;
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
	void update();

public:
	UpdateTimer(Timer*timer, int timePerUpdate);

	// milliseconds since last update
	long long getUpdateLag();

	// check if timePerUpdate exceeds updateLag
	bool isTimeToUpdate();

	// ms to trim off of updateLag, use after each update
	void updated();
};




#endif