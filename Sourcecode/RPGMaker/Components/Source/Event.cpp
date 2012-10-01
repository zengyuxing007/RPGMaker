#include "../Logger.h"
#include "../Headers/Event.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Create an event with ID <i>ID</i>, type <i>type</i> and trigger <i>trigger</i>.
*/
Event::Event(QString ID, Type type, Trigger trigger)
	: _name(ID), _type(type), _trigger(trigger),
	_limit(-1), _object(0), _followup(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an event with ID " + ID);
		logger->setStatus(true);
}

/**
*	Standard destructor.
*/
Event::~Event() {
	emit deleteSignal(_name);
}

/**
*	Get type of the event.
*/
Event::Type Event::getType() const {
	return _type;
}

/**
*	Get activation limit of the event.
*/
int Event::getLimit() const {
	return _limit;
}

/**
*	Get the name of the event.
*/
QString Event::getName() const {
	return _name;
}

/**
*	Get the object associated with the event.
*/
Object* Event::getObject() const {
	return _object;
}

/**
*	Get the trigger of the event.
*/
Event::Trigger Event::getTrigger() const {
	return _trigger;
}

/**
*	Get the follow up event.
*/
Event* Event::getFollowup() const {
	return _followup;
}

/**
*	Get the requirement event with the name <i>name</i> from the event.
*/
Event* Event::getRequirement(QString name) const {
	return getComponent<Event>(_requirements, name);
}

/**
*	Get the list of requirement events.
*/
QVector<Event*> Event::getRequirements() const {
	return _requirements;
}

/**
*	Set the name of the event.
*/
void Event::setName(QString ID) {
	if (ID == QString())
		throw ProjectException("An event should not have an empty ID.");

	_name = ID;
	Logger *logger = Logger::instance();
	logger->addMessage("Changed ID of event to " + ID);
	logger->setStatus(true);
}

/**
*	Set the trigger of the event.
*/
void Event::setTrigger(Event::Trigger trigger) {
	_trigger = trigger;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed trigger of event " + _name);
	logger->setStatus(true);
}

/**
*	Set the activation limit of the event.
*/
void Event::setLimit(int limit) {
	if (limit < -1)
		throw ProjectException("The activation limit of an event cannot be smaller than -1.");

	_limit = limit;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed activation limit of event " + _name);
	logger->setStatus(true);
}

/**
*	Set the object associated with the event.
*/
void Event::setObject(Object *object) {
	if (!object)
		throw ProjectException("Cannot set a null object.");

	if (!!_object)
		disconnect(_object, 0, this, 0);

	_object = object;
	connect(object, SIGNAL(deleteSignal(QString)), this, SLOT(objectDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Changed object of event " + _name);
	logger->setStatus(true);
}

/**
*	Set the follow up event.
*/
void Event::setFollowup(Event *event) {
	if (!event)
		throw ProjectException("Cannot set a null followup event.");

	if (event == this)
		throw ProjectException("An event canot followup itself.");

	if (!!getRequirement(event->getName()))
		throw ProjectException("An event cannot have a followup event that is also a required event.");

	if (!!_followup)
		disconnect(_followup, 0, this, 0);

	_followup = event;
	connect(_followup, SIGNAL(deleteSignal(QString())), this, SLOT(eventDeleted(QString())));

	Logger *logger = Logger::instance();
	logger->addMessage("Set followup event of event " + _name);
	logger->setStatus(true);
}

/**
*	Replace the entire list of requirements of the event.
*/
void Event::setRequirements(QVector<Event*> requirements) {
	for (int i = 0; i < _requirements.size(); i++)
		disconnect(_requirements.at(i), 0, this, 0);
	_requirements.clear();

	_requirements = requirements;
	for (int i = 0; i < _requirements.size(); i++)
		connect(_requirements.at(i), SIGNAL(deleteSignal(QString)), this, SLOT(eventDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Replaced set of requirements of event " + _name);
	logger->setStatus(true);
}

/**
*	Add a requiremenent to the event.
*/
void Event::addRequirement(Event *event) {
	if (!event)
		throw ProjectException("Cannot add a null requirement event.");

	if (event == this)
		throw ProjectException("An event cannot have itself as a requirement.");

	if (!!getRequirement(event->getName()) || (event == _followup))
		throw ProjectException("Cannot add a requirement event if it is already registered.");

	_requirements.append(event);
	connect(event, SIGNAL(deleteSignal(QString)), this, SLOT(eventDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Added a requirement event to event " + _name);
	logger->setStatus(true);
}

/**
*	Remove the object from the event.
*/
void Event::unsetObject() {
	if (!!_object)
		disconnect(_object, 0, this, 0);

	_object = 0;

	Logger *logger = Logger::instance();
	logger->addMessage("Removed object from event " + _name);
	logger->setStatus(true);
}

/**
*	Remove the follow up event from the event.
*/
void Event::unsetFollowup() {
	if (!!_followup)
		disconnect(_followup, 0, this, 0);

	_followup = 0;

	Logger *logger = Logger::instance();
	logger->addMessage("Removed followup event from event " + _name);
	logger->setStatus(true);
}

/**
*	This is called when the associated object is deleted.
*/
void Event::objectDeleted(QString name) {
	if (!!_object && (_object->getName() == name))
		unsetObject();
}

/**
*	Remove the requirement with the specified event from the event.
*/
void Event::removeRequirement(QString name) {
	Event *event = removeComponentAndLog<Event>(_requirements, name, "requirement", "event");

	if (!!event)
		delete event;
}

/**
*	This is called when the follow up event is deleted.
*/
void Event::eventDeleted(QString name) {
	if (!!getRequirement(name))
		removeRequirement(name);

	if (!!_followup && (_followup->getName() == name))
		_followup = 0;
}

/**
*	Translate the event from XML.
*/
void Event::translateFromXML(QDomElement element, Event *event) {
	QDomElement limitElement = element.firstChildElement("limit");
	event->setLimit(limitElement.attributeNode("value").value().toInt());
}

/**
*	Translate the event to XML and append it to the XML-document.
*/
void Event::translateToXML(Event *event, QDomDocument &document, QDomElement &parent) {
	parent.setAttribute("ID", event->getName());

	// Translate type
	QDomElement typeElement = document.createElement("type");
	switch(event->getType()) {
	case Event::TEXT:
		typeElement.setAttribute("value", "TEXT");
		break;
	case Event::PORTAL:
		typeElement.setAttribute("value", "PORTAL");
		break;
	case Event::BATTLE:
		typeElement.setAttribute("value", "BATTLE");
		break;
	}
	parent.appendChild(typeElement);

	// Translate trigger
	QDomElement triggerElement = document.createElement("trigger");
	switch (event->getTrigger()) {
	case Event::TOUCH:
		triggerElement.setAttribute("value", "TOUCH");
		break;
	case Event::INTERACT:
		triggerElement.setAttribute("value", "INTERACT");
		break;
	case Event::NONE:
		triggerElement.setAttribute("value", "NONE");
		break;
	}
	parent.appendChild(triggerElement);

	// Translate limit
	QDomElement limitElement = document.createElement("limit");
	limitElement.setAttribute("value", event->getLimit());
	parent.appendChild(limitElement);

	// Translate follow up event
	if (!!event->getFollowup()) {
		QDomElement followupElement = document.createElement("followupEvent");
		followupElement.setAttribute("name", event->getFollowup()->getName());
		parent.appendChild(followupElement);
	}

	// Translate requirements
	QDomElement requirementsElement = document.createElement("requirements");
	QVector<Event*> requirements = event->getRequirements();
	for (int i = 0; i < requirements.size(); i++) {
		QDomElement requirementElement = document.createElement("event");
		requirementElement.setAttribute("name", requirements.at(i)->getName());
		requirementsElement.appendChild(requirementElement);
	}
	parent.appendChild(requirementsElement);

	// Translate object
	if (!!event->getObject()) {
		QDomElement objectElement = document.createElement("object");
		objectElement.setAttribute("name", event->getObject()->getName());
		parent.appendChild(objectElement);
	}
}