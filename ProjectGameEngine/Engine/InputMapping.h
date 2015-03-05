
#pragma once


#include <list>
#include <map>

#include "InputConstants.h"
#include "InputContext.h"


// temporary location
class PausedInputContext : public InputContext {
public:
	PausedInputContext();

	virtual bool onNotify(KeyboardInput) override;
};




/*
	InputMapper is responsible for all InputContexts

	InputMapper uses the observer pattern where it is the subject and InputContexts are the observers
*/
class InputMapper {
private:
	std::map<InputContextType, InputContext*>	_inputContexts;
	std::list<InputContext*>					_activeContexts;

public:
	~InputMapper();

	/* 
		register a new context
		the context will remain inactive until activated
	*/
	void registerContext(InputContext*);

	// activate a context based on its InputContextType
	bool activateContext(InputContextType);

	// deactivate a context based on its InputContextType
	bool deactivateContext(InputContextType);

	// notify all active contexts about a key press
	void notify(KeyboardInput) const;

};