#include "InputMapping.h"

#include "InputFetcher.h"




// ---- InputMapper ----


InputMapper::~InputMapper() {
	for (auto it : _inputContexts) {
		delete it.second.context;
	}
}


void InputMapper::registerContext(InputContext* context, int priority) {
	auto prioContext = PriorityInputContext(context, priority);
	auto old = _inputContexts[context->getInputContextId()];
	if (old.context != nullptr){
		// context was already registered, remove it before re-registering
		_inputContexts.erase(old.context->getInputContextId());
	}
	// insert
	_inputContexts[context->getInputContextId()] = prioContext;
}

bool InputMapper::activateContext(int contextId) {
	auto prioContext = _inputContexts[contextId];
	if (prioContext.context != nullptr) {
		_activeContexts.insert(prioContext);
		return true;
	}
	return false;
}

bool InputMapper::deactivateContext(int contextId) {
	auto prioContext = _inputContexts[contextId];
	if (prioContext.context != nullptr) {
		for (auto it : _activeContexts) {
			if (it.context == prioContext.context) {
				_activeContexts.erase(it);
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
		for (auto prioContext : _activeContexts) {
			// stop after highest priority context that uses the key has been informed
			if (prioContext.context->onNotify(input))
				break;
		}
	}
}
