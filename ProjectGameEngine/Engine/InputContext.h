#pragma once


#include "InputConstants.h"

struct KeyboardInput;

/*
	abstract base class for InputContexts
*/
class InputContext {
private:
	static int _nextId;

protected:
	int _inputContextId;

public:
	InputContext() : _inputContextId(_nextId) { _nextId++; }
	virtual ~InputContext() {}

	// get ID of the InputContext
	int getInputContextId() const { return _inputContextId; }
	 
	/* 
		handle the given KeyboardInput
		should return true if the KeyboardInput is used and false otherwise
	*/
	virtual bool onNotify(const KeyboardInput&) = 0;

	// operator overloading
	bool operator==	(const InputContext& rhs) const { return _inputContextId == rhs._inputContextId; }
	bool operator<	(const InputContext& rhs) const { return _inputContextId < rhs._inputContextId; }
	bool operator<=	(const InputContext& rhs) const { return _inputContextId <= rhs._inputContextId; }
	bool operator>	(const InputContext& rhs) const { return _inputContextId > rhs._inputContextId; }
	bool operator>=	(const InputContext& rhs) const { return _inputContextId >= rhs._inputContextId; }
};