#include "../Logger.h"
#include "../Headers/EventSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Standard constructor.
*/
EventSystem::EventSystem()
	: _textEvents(QVector<TextEvent*>(0)),
	_portalEvents(QVector<PortalEvent*>(0)),
	_battleEvents(QVector<BattleEvent*>(0)) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created the event system");
		logger->setStatus(true);
}

/**
*	Standard destructor.
*/
EventSystem::~EventSystem() {
	for (int i = 0; i < _textEvents.size(); i++) {
		delete _textEvents.at(i);
	}
	_textEvents.clear();

	for (int i = 0; i < _portalEvents.size(); i++) {
		delete _portalEvents.at(i);
	}
	_portalEvents.clear();

	for (int i = 0; i < _battleEvents.size(); i++) {
		delete _battleEvents.at(i);
	}
	_battleEvents.clear();
}

/**
*	Get all events.
*/
QVector<Event*> EventSystem::getEvents() const {
	QVector<Event*> events(0);

	for (int i = 0; i < _textEvents.size(); i++) {
		events.append(_textEvents.at(i));
	}

	for (int i = 0; i < _portalEvents.size(); i++) {
		events.append(_portalEvents.at(i));
	}

	for (int i = 0; i < _battleEvents.size(); i++) {
		events.append(_battleEvents.at(i));
	}

	return events;
}

/**
*	Get all text events.
*/
const QVector<TextEvent*>& EventSystem::getTextEvents() const {
	return _textEvents;
}

/**
*	Get all portal events.
*/
const QVector<PortalEvent*>& EventSystem::getPortalEvents() const {
	return _portalEvents;
}

/**
*	Get all battle events.
*/
const QVector<BattleEvent*>& EventSystem::getBattleEvents() const {
	return _battleEvents;
}

/**
*	Get event with name <i>name</i>.
*/
Event* EventSystem::getEvent(QString name) const {
	for (int i = 0; i < _textEvents.size(); i++) {
		if (_textEvents.at(i)->getName() == name)
			return _textEvents.at(i);
	}

	for (int i = 0; i < _portalEvents.size(); i++) {
		if (_portalEvents.at(i)->getName() == name)
			return _portalEvents.at(i);
	}

	for (int i = 0; i < _battleEvents.size(); i++) {
		if (_battleEvents.at(i)->getName() == name)
			return _battleEvents.at(i);
	}

	return 0;
}

/**
*	Get text event with specified name.
*/
TextEvent* EventSystem::getTextEvent(QString ID) const {
	if (ID == QString())
		throw ProjectException("Cannot search for a text event with an empty ID.");

	for (int i = 0; i < _textEvents.size(); i++) {
		TextEvent *it = _textEvents.at(i);
		if (!!it && (it->getName() == ID))
			return it;
	}

	return 0;
}

/**
*	Get portal event with specified name.
*/
PortalEvent* EventSystem::getPortalEvent(QString ID) const {
	if (ID == QString())
		throw ProjectException("Cannot search for a portal event with an empty ID.");

	for (int i = 0; i < _portalEvents.size(); i++) {
		PortalEvent *it = _portalEvents.at(i);
		if (!!it && (it->getName() == ID))
			return it;
	}

	return 0;
}

/**
*	Get battle event with specified name.
*/
BattleEvent* EventSystem::getBattleEvent(QString ID) const {
	if (ID == QString())
		throw ProjectException("Cannot search for a battle event with an empty ID.");

	for (int i = 0; i < _battleEvents.size(); i++) {
		BattleEvent *it = _battleEvents.at(i);
		if (!!it && (it->getName() == ID))
			return it;
	}

	return 0;
}

/**
*	Add event to the system.
*/
void EventSystem::addEvent(Event *event) {
	if (!event)
		throw ProjectException("Cannot add a null event.");

	if (!!getEvent(event->getName()))
		throw ProjectException("An event with the same name already exists.");

	switch (event->getType()) {
	case Event::TEXT:
		addTextEvent(dynamic_cast<TextEvent*>(event));
		break;
	case Event::PORTAL:
		addPortalEvent(dynamic_cast<PortalEvent*>(event));
		break;
	case Event::BATTLE:
		addBattleEvent(dynamic_cast<BattleEvent*>(event));
		break;
	}
}

/**
*	 Add a text event to the system.
*/
void EventSystem::addTextEvent(TextEvent *event) {
	if (!event)
		throw ProjectException("Cannot add a null text event.");

	if (!!getEvent(event->getName()))
		throw ProjectException("An event with the same name already exists.");

	_textEvents.append(event);

	Logger *logger = Logger::instance();
	logger->addMessage("Added text event with ID " + event->getName());
	logger->setStatus(true);
}

/**
*	Add a portal event to the system.
*/
void EventSystem::addPortalEvent(PortalEvent *event) {
	if (!event)
		throw ProjectException("Cannot add a null portal event.");

	if (!!getEvent(event->getName()))
		throw ProjectException("An event with the same name already exists.");

	_portalEvents.append(event);

	Logger *logger = Logger::instance();
	logger->addMessage("Added portal event with ID " + event->getName());
	logger->setStatus(true);
}

/**
*	Add a battle event to the system.
*/
void EventSystem::addBattleEvent(BattleEvent *event) {
	if (!event)
		throw ProjectException("Cannot add a null battle event.");

	if (!!getEvent(event->getName()))
		throw ProjectException("An event with the same name already exists.");

	_battleEvents.append(event);

	Logger *logger = Logger::instance();
	logger->addMessage("Added battle event with ID " + event->getName());
	logger->setStatus(true);
}

/**
*	Remove event with name <i>name</i> from the system.
*/
void EventSystem::removeEvent(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an event with an empty name.");

	removeTextEvent(name);
	removePortalEvent(name);
	removeBattleEvent(name);
}

/**
*	Remove text event with name <i>name</i> from the system.
*/
void EventSystem::removeTextEvent(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a text event with an empty name.");

	for (int i = 0; i < _textEvents.size(); i++) {
		TextEvent *it = _textEvents.at(i);
		if (!!it && (it->getName() == name)) {
			_textEvents.remove(i);
			delete it;
			return;
		}
	}
}

/**
*	Remove portal event with name <i>name</i> from the system.
*/
void EventSystem::removePortalEvent(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a portal event with an empty name.");

	for (int i = 0; i < _portalEvents.size(); i++) {
		PortalEvent *it = _portalEvents.at(i);
		if (!!it && (it->getName() == name)) {
			_portalEvents.remove(i);
			delete it;
			return;
		}
	}
}

/**
*	Remove battle event with name <i>name</i> from the system.
*/
void EventSystem::removeBattleEvent(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a battle event with an empty name.");

	for (int i = 0; i < _battleEvents.size(); i++) {
		BattleEvent *it = _battleEvents.at(i);
		if (!!it && (it->getName() == name)) {
			_battleEvents.remove(i);
			delete it;
			return;
		}
	}
}

/**
*	Create an event system and return a pointer to it.
*/
EventSystem* EventSystem::create() {
	return new EventSystem();
}

/**
*	Create and translate an event system from XML and return a pointer to it.
*/
EventSystem* EventSystem::translateFromXML(QDomElement element) {
	EventSystem *system = create();

	QDomNodeList textEvents = element.elementsByTagName("textEvent");
	for (int i = 0; i < textEvents.size(); i++) {
		QDomElement textEvent = textEvents.at(i).toElement();
		TextEvent *event = TextEvent::translateFromXML(textEvent);
		system->addTextEvent(event);
	}

	QDomNodeList portalEvents = element.elementsByTagName("portalEvent");
	for (int i = 0; i < portalEvents.size(); i++) {
		QDomElement portalEvent = portalEvents.at(i).toElement();
		PortalEvent *event = PortalEvent::translateFromXML(portalEvent);
		system->addPortalEvent(event);
	}

	QDomNodeList battleEvents = element.elementsByTagName("battleEvent");
	for (int i = 0; i < battleEvents.size(); i++) {
		QDomElement battleEvent = battleEvents.at(i).toElement();
		BattleEvent *event = BattleEvent::translateFromXML(battleEvent);
		system->addBattleEvent(event);
	}

	return system;
}

/**
*	Translate the eventsystem to XML and append it to the XML-document.
*/
void EventSystem::translateToXML(EventSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("events");

	QVector<TextEvent*> textEvents = system->getTextEvents();
	for (int i = 0; i < textEvents.size(); i++) {
		TextEvent::translateToXML(textEvents.at(i), document, systemElement);
	}

	QVector<PortalEvent*> portalEvents = system->getPortalEvents();
	for (int i = 0; i < portalEvents.size(); i++) {
		PortalEvent::translateToXML(portalEvents.at(i), document, systemElement);
	}

	QVector<BattleEvent*> battleEvents = system->getBattleEvents();
	for (int i = 0; i < battleEvents.size(); i++) {
		BattleEvent::translateToXML(battleEvents.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}

/**
*	This is called when a text event is deleted.
*/
void EventSystem::textEventDeleted(QString ID) {
	for (int i = 0; i < _textEvents.size(); i++) {
		TextEvent *it = _textEvents.at(i);
		if (it->getName() == ID)
			_textEvents.remove(i);
	}
}

/**
*	This is called when a portal event is deleted.
*/
void EventSystem::portalEventDeleted(QString ID) {
	for (int i = 0; i < _portalEvents.size(); i++) {
		PortalEvent *it = _portalEvents.at(i);
		if (it->getName() == ID)
			_portalEvents.remove(i);
	}
}

/**
*	This is called when a battle event is deleted.
*/
void EventSystem::battleEventDeleted(QString ID) {
	for (int i = 0; i < _battleEvents.size(); i++) {
		BattleEvent *it = _battleEvents.at(i);
		if (it->getName() == ID)
			_battleEvents.remove(i);
	}
}