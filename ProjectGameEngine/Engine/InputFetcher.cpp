#include "InputFetcher.h"


void InputFetcher::updateKeyboard() {
	// get state
	//SDL_PumpEvents();	// update the keyboard state // unnecessary since we use pollevent for getting quit events
	_rawState = SDL_GetKeyboardState(0); // might allocate memory, look it up

	// begin working on _currentState
	switchPointers();
	_currentState->clear();


	// update
	updateAllKeys();
}


std::set<KeyboardInput, KeyboardInput_LT>*	InputFetcher::getInput() const {
	return _currentState;
}




void InputFetcher::switchPointers() {
	if (_currentState == &_keyboardState[0]) {
		_currentState = &_keyboardState[1];
		_previousState = &_keyboardState[0];
	}
	else {
		_currentState = &_keyboardState[0];
		_previousState = &_keyboardState[1];
	}
}



void InputFetcher::updateKey(SDL_Scancode scancode) {
	// check the state
	bool pressed = _rawState[scancode];
	if (!pressed)
		return;
	auto keyState = KeyboardInput(static_cast<KeyboardKey>(scancode), std::move(pressed));

	// check if it was pressed before
	auto it = _previousState->find(keyState);
	if (it != _previousState->end()) // button was previously pressed
		keyState.repeat = true;

	// insert into current
	_currentState->insert(std::move(keyState));
}




void InputFetcher::updateAllKeys() {
	// NUMERIC
	updateKey(SDL_SCANCODE_0);
	updateKey(SDL_SCANCODE_1);
	updateKey(SDL_SCANCODE_2);
	updateKey(SDL_SCANCODE_3);
	updateKey(SDL_SCANCODE_4);
	updateKey(SDL_SCANCODE_5);
	updateKey(SDL_SCANCODE_6);
	updateKey(SDL_SCANCODE_7);
	updateKey(SDL_SCANCODE_8);
	updateKey(SDL_SCANCODE_9);

	// ALPHABETIC
	updateKey(SDL_SCANCODE_A);
	updateKey(SDL_SCANCODE_B);
	updateKey(SDL_SCANCODE_C);
	updateKey(SDL_SCANCODE_D);
	updateKey(SDL_SCANCODE_E);
	updateKey(SDL_SCANCODE_F);
	updateKey(SDL_SCANCODE_G);
	updateKey(SDL_SCANCODE_H);
	updateKey(SDL_SCANCODE_I);
	updateKey(SDL_SCANCODE_J);
	updateKey(SDL_SCANCODE_K);
	updateKey(SDL_SCANCODE_L);
	updateKey(SDL_SCANCODE_M);
	updateKey(SDL_SCANCODE_N);
	updateKey(SDL_SCANCODE_O);
	updateKey(SDL_SCANCODE_P);
	updateKey(SDL_SCANCODE_Q);
	updateKey(SDL_SCANCODE_R);
	updateKey(SDL_SCANCODE_S);
	updateKey(SDL_SCANCODE_T);
	updateKey(SDL_SCANCODE_U);
	updateKey(SDL_SCANCODE_V);
	updateKey(SDL_SCANCODE_W);
	updateKey(SDL_SCANCODE_X);
	updateKey(SDL_SCANCODE_Y);
	updateKey(SDL_SCANCODE_Z);

	// ARROWS
	updateKey(SDL_SCANCODE_UP);
	updateKey(SDL_SCANCODE_DOWN);
	updateKey(SDL_SCANCODE_LEFT);
	updateKey(SDL_SCANCODE_RIGHT);

	// F-KEYS
	updateKey(SDL_SCANCODE_F1);
	updateKey(SDL_SCANCODE_F2);
	updateKey(SDL_SCANCODE_F3);
	updateKey(SDL_SCANCODE_F4);
	updateKey(SDL_SCANCODE_F5);
	updateKey(SDL_SCANCODE_F6);
	updateKey(SDL_SCANCODE_F7);
	updateKey(SDL_SCANCODE_F8);
	updateKey(SDL_SCANCODE_F9);
	updateKey(SDL_SCANCODE_F10);
	updateKey(SDL_SCANCODE_F11);
	updateKey(SDL_SCANCODE_F12);

	// KEYPAD
	updateKey(SDL_SCANCODE_KP_0);
	updateKey(SDL_SCANCODE_KP_1);
	updateKey(SDL_SCANCODE_KP_2);
	updateKey(SDL_SCANCODE_KP_3);
	updateKey(SDL_SCANCODE_KP_4);
	updateKey(SDL_SCANCODE_KP_5);
	updateKey(SDL_SCANCODE_KP_6);
	updateKey(SDL_SCANCODE_KP_7);
	updateKey(SDL_SCANCODE_KP_8);
	updateKey(SDL_SCANCODE_KP_9);
	updateKey(SDL_SCANCODE_KP_COLON);
	updateKey(SDL_SCANCODE_KP_ENTER);
	updateKey(SDL_SCANCODE_KP_PLUS);
	updateKey(SDL_SCANCODE_KP_MINUS);
	updateKey(SDL_SCANCODE_KP_MULTIPLY);
	updateKey(SDL_SCANCODE_KP_DIVIDE);

	// MODIFIERS
	updateKey(SDL_SCANCODE_LALT);
	updateKey(SDL_SCANCODE_RALT);
	updateKey(SDL_SCANCODE_LCTRL);
	updateKey(SDL_SCANCODE_RCTRL);
	updateKey(SDL_SCANCODE_LSHIFT);
	updateKey(SDL_SCANCODE_RSHIFT);

	// MISC
	updateKey(SDL_SCANCODE_SPACE);
	updateKey(SDL_SCANCODE_BACKSLASH);
	updateKey(SDL_SCANCODE_DELETE);
	updateKey(SDL_SCANCODE_END);
	updateKey(SDL_SCANCODE_ESCAPE);
	updateKey(SDL_SCANCODE_HOME);
	updateKey(SDL_SCANCODE_INSERT);
	updateKey(SDL_SCANCODE_PAGEUP);
	updateKey(SDL_SCANCODE_PAGEDOWN);
	updateKey(SDL_SCANCODE_PAUSE);
	updateKey(SDL_SCANCODE_PRINTSCREEN);
	updateKey(SDL_SCANCODE_SEMICOLON);
	updateKey(SDL_SCANCODE_SEPARATOR);
	updateKey(SDL_SCANCODE_DECIMALSEPARATOR);

	// ENTER
	updateKey(SDL_SCANCODE_RETURN);
	updateKey(SDL_SCANCODE_RETURN2);
}