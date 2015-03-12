#pragma once

#include <string>
#include <unordered_map>

enum class EventType {
	NONE,
	PLAYER_COLLISION,
	GAME_NEW,
	GAME_END,
	GAME_LOGIC_PAUSE,
	GAME_LOGIC_UNPAUSE,

	PLAY_MUSIC,
	PLAY_SOUND
};


struct GameEvent {
	EventType	type;
	int			eventId;
	std::string	data;

	GameEvent(EventType type, std::string data = "", int id = -1) 
		: type(type), eventId(id), data(data) {}
};


// add game events
class EventQueue {
private:
	std::unordered_map<EventType, GameEvent> _eventCollection;

	// constructors
private:
	EventQueue() {}
	EventQueue(const EventQueue&) = delete;
	EventQueue& operator=(const EventQueue&) = delete;

public:
	static EventQueue& getInstance() {
		static EventQueue instance;
		return instance;
	}

	// add an event to the queue
	void add(GameEvent ev) {
		_eventCollection.insert(std::make_pair(ev.type, ev));
	}

	// poll for a specific type of event
	GameEvent poll(EventType type) {
		auto evit = _eventCollection.find(type);

		if (evit == _eventCollection.end()) // not found
			return GameEvent(EventType::NONE);

		auto ev = evit->second;
		_eventCollection.erase(evit);
		return ev;
	}
};