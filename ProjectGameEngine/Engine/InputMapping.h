
#pragma once


#include <list>
#include <map>
#include <set>

#include "InputConstants.h"
#include "InputContext.h"
#include "InputFetcher.h"

// for organizing InputContexts in a set
struct PriorityInputContext {
	InputContext*	context;
	int				priority;

	PriorityInputContext(InputContext* context = nullptr, int priority = 0) : context(context), priority(priority) {}
};

// less-than functor for KeyboardInput
struct PriorityInputContext_LT {
	bool operator()(const PriorityInputContext& lhs, const PriorityInputContext& rhs) { 
		return lhs.priority > rhs.priority; 
	}
};

// for reading comprehension
typedef std::set<PriorityInputContext, PriorityInputContext_LT> PriorityContextSet;


/*
	InputMapper is responsible for all InputContexts

	InputMapper uses the observer pattern where it is the subject and InputContexts are the observers
*/
class InputMapper {
private:
	InputFetcher						_fetcher;
	std::map<int, PriorityInputContext>	_inputContexts;
	PriorityContextSet					_activeContexts;

private:
	InputMapper() {}
	InputMapper(const InputMapper&) = delete;
	InputMapper operator=(const InputMapper&) = delete;

public:
	static InputMapper& getInstance();

	~InputMapper();

	/* 
		register a new context
		the context will remain inactive until activated
		default priority is 0
	*/
	void registerContext(InputContext* context, int priority = 0);

	/*
		unregister an existing context
	*/
	void unregisterContext(InputContext* context);
	/* 
		activate a context based on its id
		return false if context doesnt exist
	*/
	bool activateContext(InputContext* context);

	/* 
		deactivate a context based on its id
		return false if context doesnt exist
	*/
	bool deactivateContext(InputContext* context);

	// notify all active contexts about a key press
	void notify();

};