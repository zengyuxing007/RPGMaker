#include "../Logger.h"
#include "../Headers/TextEvent.h"

using namespace projectcomponents;

/**
*	Constructs a text event with name <i>name</i> and trigger <i>trigger</i>.
*/
TextEvent::TextEvent(QString name, Event::Trigger trigger)
	: Event(name, Event::TEXT, trigger), _messages(QVector<QString>(0)) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created text event with ID " + _name);
		logger->setStatus(true);
}

/**
*	Standard destructor.
*/
TextEvent::~TextEvent() {

	Logger *logger = Logger::instance();
	logger->addMessage("Destroyed text event " + _name);
	logger->setStatus(true);
}

/**
*	Empty function...
*/
void TextEvent::execute() {

}

/**
*	Get all messages associated with this text event.
*/
QVector<QString> TextEvent::getMessages() const {
	return _messages;
}

/**
*	Get the message at position <i>position</i>.
*	The position must be valid, a <i>ProjectException</i> is thrown otherwise.
*/
QString TextEvent::getMessage(int position) const {
	if ((position < 0) || (position >= _messages.size()))
		throw ProjectException("The index " + QString::number(position) + " does not exist in the text event.");

	return _messages.at(position);
}

/**
*	Add a message to the end of this text event.
*/
void TextEvent::addMessage(QString message) {
	_messages.append(message);

	Logger *logger = Logger::instance();
	logger->addMessage("Added message to text event " + _name);
	logger->setStatus(true);
}

/**
*	Edit the message with text <i>message</i> specified at position <i>position</i>.
*	The position must be valid, a <i>ProjectException</i> is thrown otherwise.
*/
void TextEvent::editMessage(QString message, int position) {
	if ((position < 0) || (position >= _messages.size()))
		throw ProjectException("The index " + QString::number(position) + " does not exist in the text event.");

	_messages.replace(position, message);

	Logger *logger = Logger::instance();
	logger->addMessage("Edited message of text event " + _name);
	logger->setStatus(true);
}

/**
*	Remove the message specified at position <i>position</i>.
*	The position must be valid, a <i>ProjectException</i> is thrown otherwise.
*/
void TextEvent::removeMessage(int position) {
	if ((position < 0) || (position >= _messages.size()))
		throw ProjectException("The index " + QString::number(position) + " does not exist in the text event.");

	_messages.remove(position);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed message from text event " + _name);
	logger->setStatus(true);
}

/**
*	Delete all messages of this text event.
*/
void TextEvent::removeAllMessages() {
	_messages.clear();

	Logger *logger = Logger::instance();
	logger->addMessage("Removed all messages from text event " + _name);
	logger->setStatus(true);
}

/**
*	Create a text event with name <i>name</i> and trigger <i>trigger</i> and return a pointer to this instance.
*	The name may not be empty, a <i>ProjectException</i> is thrown otherwise.
*/
TextEvent* TextEvent::create(QString name, Event::Trigger trigger) {
	if (name == QString())
		throw ProjectException("Cannot create a text event with an empty ID.");

	return new TextEvent(name, trigger);
}

/**
* Translate the text event from XML.
*/
TextEvent* TextEvent::translateFromXML(QDomElement element) {
	QString ID = element.attributeNode("ID").value();
	QDomElement typeElement = element.firstChildElement("type");
	QString typeString = typeElement.attributeNode("value").value();

	if (typeString != "TEXT")
		throw ProjectException("The translation of a text event should have type TEXT instead of " + typeString + ".");

	Event::Trigger trigger = Event::NONE;
	QString triggerString = element.firstChildElement("trigger").attributeNode("value").value();
	if (triggerString == "TOUCH")
		trigger = Event::TOUCH;
	else if (triggerString == "INTERACT")
		trigger = Event::INTERACT;
	else if (triggerString == "NONE")
		trigger = Event::NONE;

	TextEvent *event = create(ID, trigger);
	Event::translateFromXML(element, event);

	QDomNodeList textElements = element.elementsByTagName("text");
	for (int i = 0; i < textElements.size(); i++) {
		QDomElement textElement = textElements.at(i).toElement();
		event->addMessage(textElement.text());
	}

	return event;
}

/**
*	Translate the text event to XML.
*/
void TextEvent::translateToXML(TextEvent *event, QDomDocument &document, QDomElement &parent) {
	QDomElement eventElement = document.createElement("textEvent");

	Event::translateToXML(event, document, eventElement);

	QVector<QString> messages = event->getMessages();
	for (int i = 0; i < messages.size(); i++) {
		QDomElement textElement = document.createElement("text");
		QDomText text = document.createTextNode(messages.at(i));
		textElement.appendChild(text);
		eventElement.appendChild(textElement);
	}

	parent.appendChild(eventElement);
}