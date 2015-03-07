#include "InputMapping.h"





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
		_activeContexts.sort(inputContext_lessThan);
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

void InputMapper::notify() {
	_fetcher.updateKeyboard();
	auto inputSet = _fetcher.getInput();
	for (auto input : *inputSet) {
		for (auto context : _activeContexts) {
			// stop after highest priority context that uses the key has been informed
			if (context->onNotify(input))
				break;
		}
	}
}
