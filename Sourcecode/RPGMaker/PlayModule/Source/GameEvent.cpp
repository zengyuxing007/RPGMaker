#include "../Headers/GameEvent.h"

using namespace playmodule;

GameEvent::GameEvent(Event *event)
	: _event(event), _followup(0), _requirements(0),
	_executed(false), _executionsRemaining(0) {

		_executionsRemaining = _event->getLimit();
}

GameEvent::~GameEvent() {}

Event* GameEvent::getEvent() const {
	return _event;
}

Event::Type GameEvent::getType() const {
	return _event->getType();
}

Event::Trigger GameEvent::getTrigger() const {
	return _event->getTrigger();
}

int GameEvent::getExecutionLimit() const {
	return _event->getLimit();
}

GameEvent* GameEvent::getFollowupEvent() const {
	return _followup;
}

void GameEvent::setFollowupEvent(GameEvent *event) {
	_followup = event;
}

void GameEvent::addRequirementEvent(GameEvent *event) {
	_requirements.append(event);
}

void GameEvent::execute() {
	_executed = true;
	
	if (_executionsRemaining > 0)
		_executionsRemaining--;
}

bool GameEvent::isExecuted() const {
	return _executed;
}

bool GameEvent::executionPermission() const {

	if (_executionsRemaining == 0)
		return false;

	for (int i = 0; i < _requirements.size(); i++) {
		if (!_requirements.at(i)->isExecuted())
			return false;
	}

	return true;
}