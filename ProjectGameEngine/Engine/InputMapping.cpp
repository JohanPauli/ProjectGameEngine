#include "InputMapping.h"

// ---- Menu ----

PausedInputContext::PausedInputContext() : InputContext(InputContextType::MENU) {}


bool PausedInputContext::onNotify(KeyboardInput key) {
	switch (key) {
	case KeyboardInput::ALT_RIGHT:

		return true;
	case KeyboardInput::W:

		return true;
	case KeyboardInput::F:

		return true;
	case KeyboardInput::U:

		return true;
	case KeyboardInput::C:

		return true;
	case KeyboardInput::K:

		return true;
	case KeyboardInput::I:

		return true;
	case KeyboardInput::T:

		return true;
	default: return false;
	}
}




// ---- InputMapper ----


InputMapper::~InputMapper() {
	for (auto it : _inputContexts) {
		delete it.second;
	}
}


void InputMapper::registerContext(InputContext* context) {
	if (_inputContexts[context->getType()] != nullptr)
		return;
	_inputContexts[context->getType()] = context;
}

bool InputMapper::activateContext(InputContextType type) {
	auto context = _inputContexts[type];
	if (context != nullptr) {
		_activeContexts.emplace_front(context);
		_activeContexts.sort(compareInputContext);
		return true;
	}
	return false;
}

bool InputMapper::deactivateContext(InputContextType type) {
	auto context = _inputContexts[type];
	if (context != nullptr) {
		for (auto it : _activeContexts) {
			if (it == context) {
				_activeContexts.remove(it);
				return true;
			}
		}
	}
	return false;
}



void InputMapper::notify(KeyboardInput key) const {
	for (auto context : _activeContexts) {
		// stop after highest priority context that uses the key has been informed
		if (context->onNotify(key)) 
			return;
	}
}