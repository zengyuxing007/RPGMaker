#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/StartEvent.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Standard constructor.
*/
StartEvent::StartEvent()
	: _currency(0),
	_characters(QVector<Character*>(0)),
	_location(QPair<Environment*, QPoint>(0, QPoint(0, 0))) {
		int i = 3;
}

/**
*	Standard destructor.
*/
StartEvent::~StartEvent() {}

/**
*	Get the starting location of the game.
*/
QPair<Environment*, QPoint> StartEvent::getLocation() const {
	return _location;
}

/**
*	Get the starting amount of money the player's party will have.
*/
int StartEvent::getCurrency() const {
	return _currency;
}

/**
*	Get the Character with name <i>name</i> of the starting praty.
*	The name must not be empty, a <i>ProjectException</i> is thrown otherwise.
*	If no Character with the specified name could be found, a null-pointer is returned.
*/
Character* StartEvent::getCharacter(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for a character with an empty name.");

	return getComponent<Character>(_characters, name);
}

/**
*	Get the Character specified at position <i>position</i> in the party.
*	The position must be valid, a <i>ProjectException</i> is thrown otherwise.
*	If no Character is found a the specified position, a null-pointer is returned.
*/
Character* StartEvent::getCharacter(int position) const {
	if ((position < 0) || (position >= _characters.size()))
		throw ProjectException("The character index " + QString::number(position) + " does not exist.");

	return _characters.at(position);
}

/**
*	Get the list of characters in the starting party.
*/
QVector<Character*> StartEvent::getParty() const {
	return _characters;
}

/**
*	Set the starting location of the game to environment <i>environment</i> at location <i>location</i>.
*	The location should be valid, a <i>ProjectException</i> is thrown otherwise.
*/
void StartEvent::setLocation(Environment *environment, QPoint location) {
	if (!environment)
		throw ProjectException("Cannot set a null environment for the start event.");

	if ((location.x() < 0) || (location.x() > environment->getDimensions().width()))
		throw ProjectException("The location is out of bounds of the environment.");

	if ((location.y() < 0) || (location.y() > environment->getDimensions().height()))
		throw ProjectException("The location is out of bounds of the environment.");

	_location.first = environment;
	_location.second = location;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed location of start event");
	logger->setStatus(true);
}

/**
*	Set the starting amount of money for the player.
*	This value should be larger than 0, a <i>ProjectException</i> is thrown otherwise.
*/
void StartEvent::setCurrency(int currency) {
	if (currency < 0)
		throw ProjectException("The amount of currency should not be negative.");

	_currency = currency;

	Logger *logger = Logger::instance();
	logger->addMessage("Changed start event currency");
	logger->setStatus(true);
}

/**
*	Append a character to the party.
*	The party must not be full and the character must already be in the party.
*/
void StartEvent::addCharacter(Character *character) {
	if (!character)
		throw ProjectException("Cannot set a null character.");

	if (_characters.size() == g_maxPartysize)
		throw ProjectException("The maximum amount of characters is already in the party.");

	if (!!getCharacter(character->getName()))
		throw ProjectException("The character is already a member of the party.");

	_characters.append(character);
	connect(character, SIGNAL(deleteSignal(QString)), this, SLOT(characterDeleted(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Added character " + character->getName() + " to the party");
	logger->setStatus(true);
}

/**
*	Remove the character with the specified name from the party.
*/
void StartEvent::removeCharacter(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a character with an empty name.");

	Character *character = removeComponentAndLog<Character>(_characters, name, "character", "start event");
	if (!!character)
		disconnect(character, 0, this, 0);
}

/**
*	Remove the character at the specified position in the party, from the party.
*/
void StartEvent::removeCharacter(int position) {
	if ((position < 0) || (position >= _characters.size()))
		throw ProjectException("The character index " + QString::number(position) + " does not exist.");

	Character *character = getCharacter(position);
	if (!!character) {
		removeComponentAndLog<Character>(_characters, character->getName(), "character", "start event");
		disconnect(character, 0, this, 0);
	}
}

/**
*	Rmove the current starting location.
*/
void StartEvent::removeLocation() {
	_location.first = 0;
	_location.second = QPoint(0, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed location from start event");
	logger->setStatus(true);
}

/**
*	Create a start event and return a pointer to it.
*/
StartEvent* StartEvent::create() {

	return new StartEvent();
}

/**
*	Translate the start event from XML and return a pointer to it.
*/
StartEvent* StartEvent::translateFromXML(QDomElement &element) {
	return StartEvent::create();
}

/**
*	Translate the start event to XML and add it to the XML-document.
*/
void StartEvent::translateToXML(StartEvent *event, QDomDocument &document, QDomElement &parent) {
	QDomElement startElement = document.createElement("start");

	if (!!event->getLocation().first) {
		QDomElement locationElement = document.createElement("location");
		locationElement.setAttribute("environment", event->getLocation().first->getName());
		locationElement.setAttribute("row", event->getLocation().second.y());
		locationElement.setAttribute("column", event->getLocation().second.x());
		startElement.appendChild(locationElement);
	}

	QDomElement partyElement = document.createElement("party");
	QVector<Character*> characters = event->getParty();
	for (int i = 0; i < characters.size(); i++) {
		Character *it = characters.at(i);
		if (!!it) {
			QDomElement characterElement = document.createElement("character");
			characterElement.setAttribute("name", it->getName());
			partyElement.appendChild(characterElement);
		}
	}
	startElement.appendChild(partyElement);

	QDomElement inventoryElement = document.createElement("inventory");
	QDomElement currencyElement = document.createElement("currency");
	currencyElement.setAttribute("value", event->getCurrency());
	inventoryElement.appendChild(currencyElement);

	startElement.appendChild(inventoryElement);

	parent.appendChild(startElement);
}

/**
*	This is called when a character is deleted and will remove all pointers that are related to this character.
*/
void StartEvent::characterDeleted(QString name) {
	if (name == QString())
		return;

	for (int i = 0; i < _characters.size(); i++) {
		Character *it = _characters.at(i);
		if (!!it && (it->getName() == name))
			_characters.replace(i, 0);
	}
}