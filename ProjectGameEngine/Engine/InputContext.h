#pragma once


#include "InputConstants.h"

struct KeyboardInput;

/*
	
*/
class InputContext {
protected:
	InputContextType _type;

public:
	InputContext(InputContextType type) : _type(type) {}
	virtual ~InputContext() {}

	// get the type of context. type is used as an ID
	InputContextType getType() const { return _type; }
	 
	/* 
		handle the given KeyboardInput
		should return true if the KeyboardInput is used and false otherwise
	*/
	virtual bool onNotify(const KeyboardInput&) = 0;
};

// comparator function
inline bool inputContext_lessThan(InputContext* lhs, InputContext* rhs) { return lhs->getType() < rhs->getType(); }