#include "qfileinfo.h"

#include "../Logger.h"
#include "../ProgramDefaults.h"
#include "../Headers/Character.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

Character::Character(QString name)
	: _name(name), _class(0), _level(0),
	_abilities(QVector<QPair<Ability*, int>>(0)),
	_elements(QVector<Element*>(0)),
	_equipment(CharacterEquipment::create()) {

		Logger *logger = Logger::instance();
		logger->setStatus(true);
		logger->addMessage("Created character " + _name);
}

Character::~Character() {
	emit deleteSignal(_name);

	_abilities.clear();
	_elements.clear();
	_class = 0;
	
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Destroyed character " + _name);
}

QString Character::getName() const {
	return this->_name;
}

QString Character::getBiography() const {
	return this->_biography;
}

Class* Character::getClass() const {
	return this->_class;
}

int Character::getLevel() const {
	return this->_level;
}

Element* Character::getElement(QString name) const {
	return getComponent<Element>(_elements, name);
}

QPair<Ability*, int> Character::getAbility(QString name) const {
	return getPair<Ability, int>(_abilities, name);
}

const QVector<QPair<Ability*, int>>& Character::getAbilities() const {
	return _abilities;
}

QVector<QPair<Ability*, int>> Character::getAllAbilities() const {
	QVector<QPair<Ability*, int>> abilities(0);

	if (!!_class) {
		QVector<QPair<Ability*, int>> classAbilities = _class->getAbilities();
		for (int i = 0; i < classAbilities.size(); i++)
			abilities.append(classAbilities.at(i));
	}

	for (int i = 0; i < _abilities.size(); i++)
		abilities.append(_abilities.at(i));

	return abilities;
}

const QVector<Element*>& Character::getElements() const {
	return this->_elements;
}

QPair<QPixmap, QString> Character::getBattleImage() const {
	return _battleimage;
}

QPair<QPixmap, QString> Character::getEnvironmentImage() const {
	return _environmentimage;
}

CharacterEquipment* Character::getEquipment() const {
	return _equipment;
}

void Character::setName(QString name) {
	if (name == QString())
		throw ProjectException("A character should not have an empty name");

	this->_name = name;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed name of character to " + _name);
}

void Character::setBiography(QString newBiography) {
	this->_biography = newBiography;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed biography of character " + _name);
}

void Character::setClass(Class *newClass) {
	if (!!_class)
		disconnect(_class, 0, this, 0);

	for (int i = 0; i < _abilities.size(); i++) {
		QPair<Ability*, int> it = _abilities.at(i);
		if (!!newClass->getAbility(it.first->getName()).first) {
			_abilities.remove(i);
			i--;
		}
	}

	_class = newClass;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed class of character " + _name);
	
	connect(newClass, SIGNAL(deleteSignal(QString)), this, SLOT(deleteClass(QString)));
}

void Character::setLevel(int level) {
	this->_level = level;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed level of character " + _name);
}

void Character::setBattleImage(QString imagename) {
	QFileInfo imageinfo(g_battlersPath + imagename);
	if (!imageinfo.exists())
		throw ProjectException("The image " + imagename + "does not exist in the path " + g_battlersPath + ".");

	_battleimage.first.load(g_battlersPath + imagename);
	_battleimage.second = imagename;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed battler image of character " + _name);
}

void Character::setEnvironmentImage(QString imagename) {
	QFileInfo imageinfo(g_charactersPath + imagename);
	if (!imageinfo.exists())
		throw ProjectException("The image " + imagename + "does not exist in the path " + g_charactersPath + ".");

	_environmentimage.first.load(g_charactersPath + imagename);
	_environmentimage.second = imagename;

	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Changed environmental image of character " + _name);
}

void Character::setEquipment(CharacterEquipment *equipment) {
	if (!equipment)
		throw ProjectException("Cannot set null equipment.");

	_equipment = equipment;
}

void Character::addAbility(Ability *newAbility, int acquire) {
	QPair<Ability*, int> ability(newAbility, acquire);
	this->addAbility(ability);
}

void Character::addAbility(QPair<Ability*, int> newAbility) {
	if (!!_class && !!_class->getAbility(newAbility.first->getName()).first)
		throw ProjectException("This ability is already registered under the class of this character.");

	addPairAndLog<Ability, int>(_abilities, newAbility, "ability", "character " + _name);
	connect(newAbility.first, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAbility(QString)));
}

void Character::addElement(Element *newElement) {
	addComponentAndLog<Element>(_elements, newElement, "element", "character " + _name);
	connect(newElement, SIGNAL(deleteSignal(QString)), this, SLOT(deleteElement(QString)));
}

void Character::editAbility(QString ability, int acquire) {
	if (ability == QString())
		throw ProjectException("Cannot search for an ability with an empty name.");

	if (!getAbility(ability).first)
		throw ProjectException("The ability " + ability + " cannot be found for character " + _name);

	if ((acquire < 0) || (acquire > 99))
		throw ProjectException("The acquisition of an ability should be between 0 and 99.");

	for (QVector<QPair<Ability*, int>>::iterator it = _abilities.begin(); it != _abilities.end(); it++) {
		if (it->first->getName() == ability) {
			it->second = acquire;
			return;
		}
	}
}

void Character::removeAbility(Ability *oldAbility) {
	this->removeAbility(oldAbility->getName());
}

void Character::removeAbility(QString oldAbility) {
	QPair<Ability*, int> ability = removePairAndLog<Ability, int>(_abilities, oldAbility, "ability", "character " + _name);
	if (!!ability.first)
		disconnect(ability.first, 0, this, 0);
}

void Character::removeElement(Element *oldElement) {
	this->removeElement(oldElement->getName());
}

void Character::removeElement(QString oldElement) {
	Element *element = removeComponentAndLog<Element>(_elements, oldElement, "element", "character " + _name);
	if (!!element)
		disconnect(element, 0, this, 0);
}

void Character::unsetClass() {
	if (!!_class)
		disconnect(_class, 0, this, 0);

	this->_class = 0;
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Removed class from character " + _name);
}

Character* Character::create(QString name) {
	if (name == QString())
		throw ProjectException("A character should not have an empty name.");

	Character *character = new Character(name);
	return character;
}

Character* Character::translateFromXML(QDomElement &characterElement) {
	QString name = characterElement.attributeNode("name").value();
	Character *character = Character::create(name);

	character->setLevel(characterElement.firstChildElement("level").attributeNode("value").value().toInt());
	character->setBiography(characterElement.firstChildElement("biography").text());

	QDomElement imagesElement = characterElement.firstChildElement("images");
	QDomElement environmentElement = imagesElement.firstChildElement("environment");
	QDomElement battleElement = imagesElement.firstChildElement("battle");

	QString environmentImageName = environmentElement.attributeNode("image").value();
	QString battleImageName = battleElement.attributeNode("image").value();

	if (environmentImageName != QString())
		character->setEnvironmentImage(environmentImageName);

	if (battleImageName != QString())
		character->setBattleImage(battleImageName);

	return character;
}

void Character::translateToXML(Character *character, QDomDocument &document, QDomElement &parent) {
	QDomElement characterElement = document.createElement("character");
	characterElement.setAttribute("name", character->_name);

	QDomElement level = document.createElement("level");
	level.setAttribute("value", character->getLevel());
	characterElement.appendChild(level);

	if (character->_class) {
		QDomElement cat = document.createElement("class");
		cat.setAttribute("name", character->_class->getName());
		characterElement.appendChild(cat);
	}

	QDomElement biography = document.createElement("biography");
	QDomText text = document.createTextNode(character->_biography);
	biography.appendChild(text);
	characterElement.appendChild(biography);

	QDomElement elements = document.createElement("elements");
	for (int i = 0; i < character->_elements.size(); i++) {
		Element *it = character->_elements.at(i);
		QDomElement element = document.createElement("element");
		element.setAttribute("name", it->getName());
		elements.appendChild(element);
	}
	characterElement.appendChild(elements);

	QDomElement abilities = document.createElement("abilities");
	for (int i = 0; i < character->_abilities.size(); i++) {
		QPair<Ability*, int> it = character->_abilities.at(i);
		QDomElement ability = document.createElement("ability");
		ability.setAttribute("name", it.first->getName());
		ability.setAttribute("acquire", it.second);
		abilities.appendChild(ability);
	}
	characterElement.appendChild(abilities);

	QDomElement equipment = document.createElement("equipment");
	CharacterEquipment::translateToXML(character->_equipment, document, equipment);
	characterElement.appendChild(equipment);

	QDomElement images = document.createElement("images");
	QDomElement environment = document.createElement("environment");
	environment.setAttribute("image", character->_environmentimage.second);
	images.appendChild(environment);
	QDomElement battle = document.createElement("battle");
	battle.setAttribute("image", character->_battleimage.second);
	images.appendChild(battle);
	characterElement.appendChild(images);

	parent.appendChild(characterElement);
}

void Character::deleteAbility(QString name) {
	try {
		removeAbility(name);
	} catch (ProjectException &e) {}
}

void Character::deleteElement(QString name) {
	try {
		removeElement(name);
	} catch (ProjectException &e) {}
}

void Character::deleteClass(QString name) {
	unsetClass();
}