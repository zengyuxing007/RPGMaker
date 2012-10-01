#include "../Logger.h"
#include "../Headers/CharacterSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

CharacterSystem::CharacterSystem()
	: _characters(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created a character system");
		logger->setStatus(true);
}

CharacterSystem::~CharacterSystem() {
	for (int i = 0; i < _characters.size(); i++) {
		Character *it = _characters.at(i);

		if (!!it)
			delete it;
	}

	_characters.clear();
}

const QVector<Character*>& CharacterSystem::getCharacters() const {
	return _characters;
}

Character* CharacterSystem::getCharacter(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for a character with an empty name.");

	return getComponent<Character>(_characters, name);
}

void CharacterSystem::addCharacter(Character *character) {
	if (!character)
		throw ProjectException("Cannot add a null character.");

	_characters.append(character);

	Logger *logger = Logger::instance();
	logger->addMessage("Added character " + character->getName() + " to the project");
	logger->setStatus(true);
}

void CharacterSystem::removeCharacter(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a character with an empty name.");

	Character *character = removeComponentAndLog<Character>(_characters, name, "character", "project");

	if (!!character)
		delete character;
}

CharacterSystem* CharacterSystem::create() {
	return new CharacterSystem;
}

CharacterSystem* CharacterSystem::translateFromXML(QDomElement &element) {
	CharacterSystem *system = create();

	QDomNodeList characterElements = element.childNodes();

	for (int i = 0; i < characterElements.size(); i++) {
		QDomElement characterElement = characterElements.at(i).toElement();
		system->addCharacter(Character::translateFromXML(characterElement));
	}

	return system;
}

void CharacterSystem::translateToXML(CharacterSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement systemElement = document.createElement("characters");

	const QVector<Character*> characters = system->getCharacters();
	for (int i = 0; i < characters.size(); i++) {
		Character::translateToXML(characters.at(i), document, systemElement);
	}

	parent.appendChild(systemElement);
}