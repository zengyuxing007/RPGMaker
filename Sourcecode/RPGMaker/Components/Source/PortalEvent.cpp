#include "../Logger.h"
#include "../Headers/PortalEvent.h"

using namespace projectcomponents;

/**
*	Create a portal event with name <i>name</i> and trigger <i>trigger</i>.
*/
PortalEvent::PortalEvent(QString name, Event::Trigger trigger)
	: Event(name, Event::PORTAL, trigger), _location(0,0), _environment(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created portal event with ID " + _name);
		logger->setStatus(true);
}

/**
*	Standard destructor
*/
PortalEvent::~PortalEvent() {}

/**
*	Empty function...
*/
void PortalEvent::execute() {

}

/**
*	Get portal destination.
*/
QPoint PortalEvent::getLocation() const {
	return _location;
}

/**
*	Get destination environment.
*/
Environment* PortalEvent::getEnvironment() const {
	return _environment;
}

/**
*	Set the destination.
*/
void PortalEvent::setDestination(Environment *environment, QPoint location) {
	if (!environment)
		throw ProjectException("Cannot set a null environment as a destination.");

	QSize dimensions = environment->getDimensions();
	if ((location.x() < 0) || (location.x() >= dimensions.width()) || (location.y() < 0) || (location.y() >= dimensions.height()))
		throw ProjectException("The destination is out of bounds of the environment.");

	if (!!_environment)
		disconnect(_environment, 0, this, 0);

	_environment = environment;
	_location = location;
	connect(_environment, SIGNAL(deleteSignal(QString)), this, SLOT(environmentDeleted(QString)));
	connect(_environment, SIGNAL(resizeSignal(QString)), this, SLOT(environmentResized(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Changed destination of portal event " + _name);
	logger->setStatus(true);
}

/**
*	Remove the current destination.
*/
void PortalEvent::unsetDestination() {
	if (!!_environment)
		disconnect(_environment, 0, this, 0);

	_environment = 0;
	_location = QPoint(0,0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed destination of portal event " + _name);
	logger->setStatus(true);
}

/**
*	Create a portal event with name <i>name</i> and trigger <i>trigger</i> and return a pointer to it.
*/
PortalEvent* PortalEvent::create(QString name, Event::Trigger trigger) {
	if (name == QString())
		throw ProjectException("Cannot create a portal event with an empty ID.");

	return new PortalEvent(name, trigger);
}

/**
*	Create and translate a portal event from XML and return a pointer to it.
*/
PortalEvent* PortalEvent::translateFromXML(QDomElement element) {
	QString ID = element.attributeNode("ID").value();
	QDomElement typeElement = element.firstChildElement("type");
	QString typeString = typeElement.attributeNode("value").value();

	if (typeString != "PORTAL")
		throw ProjectException("The translation of a portal event should have type PORTAL instead of " + typeString + ".");

	Event::Trigger trigger = Event::NONE;
	QString triggerString = element.firstChildElement("trigger").attributeNode("value").value();
	if (triggerString == "TOUCH")
		trigger = Event::TOUCH;
	else if (triggerString == "INTERACT")
		trigger = Event::INTERACT;
	else if (triggerString == "NONE")
		trigger = Event::NONE;

	PortalEvent *event = PortalEvent::create(ID, trigger);
	Event::translateFromXML(element, event);

	return event;
}

/**
*	Translate portal event to XML and add to the XML-document.
*/
void PortalEvent::translateToXML(PortalEvent *event, QDomDocument &document, QDomElement &parent) {
	QDomElement eventElement = document.createElement("portalEvent");

	Event::translateToXML(event, document, eventElement);

	if (!!event->getEnvironment()) {
		QDomElement toElement = document.createElement("to");
		toElement.setAttribute("location", event->getEnvironment()->getName());
		toElement.setAttribute("row", event->getLocation().y());
		toElement.setAttribute("column", event->getLocation().x());
		eventElement.appendChild(toElement);
	}

	parent.appendChild(eventElement);
}

/**
*	This is called when the destination environment is deleted.
*/
void PortalEvent::environmentDeleted(QString name) {
	if (_environment->getName() == name)
		unsetDestination();
}

/**
*	This is called when the destination environment is resized.
*/
void PortalEvent::environmentResized(QString name) {
	if (!!_environment && (_environment->getName() == name)) {
		QSize dimensions = _environment->getDimensions();

		if ((_location.x() >= dimensions.width()) || (_location.y() >= dimensions.height()))
			unsetDestination();
	}
}