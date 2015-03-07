#pragma once

#include <SDL.h>

#include <set>

#include "InputConstants.h"


// the state of a key on the keyboard
struct KeyboardInput {
	KeyboardKey key;
	bool		pressed;	// is key pressed down
	bool		repeat;		// button was down last check

	// constructor
	KeyboardInput(KeyboardKey key, bool pressed = false, bool rep = false)
		: key(key), pressed(pressed), repeat(rep) {}
};

// less-than comparator object for KeyboardInput
struct KeyboardInput_LT {
	bool operator()(const KeyboardInput& lhs, const KeyboardInput& rhs) {
		return lhs.key < rhs.key;
	}
};



// gathers and stores the state of the keyboard
class InputFetcher {
private:
	std::set<KeyboardInput, KeyboardInput_LT>	_keyboardState[2];
	std::set<KeyboardInput, KeyboardInput_LT>*  _currentState = nullptr;
	std::set<KeyboardInput, KeyboardInput_LT>*  _previousState = nullptr;
	const Uint8*			_rawState;

private:
	void switchPointers();
	void updateKey(SDL_Scancode);
	void updateAllKeys();

public:
	// updates the current state of the keyboard
	void updateKeyboard();

	// returns all keys that were pressed last update
	std::set<KeyboardInput, KeyboardInput_LT>*	getInput() const;
};