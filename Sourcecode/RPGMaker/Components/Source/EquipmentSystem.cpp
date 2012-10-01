#include "../Logger.h"
#include "../Headers/EquipmentSystem.h"
#include "../Headers/ProjectTemplateFunctions.h"

using namespace projectcomponents;

/**
*	Standard constructor.
*/
EquipmentSystem::EquipmentSystem()
	: _armor(0), _weapons(0), _accessories(0) {

		Logger *logger = Logger::instance();
		logger->addMessage("Created an equipment system");
		logger->setStatus(true);
}

/**
*	Standard destructor.
*/
EquipmentSystem::~EquipmentSystem() {
	for (int i = 0; i < _armor.size(); i++) {
		Armor *it = _armor.at(i);

		if (!!it)
			delete it;
	}
	_armor.clear();

	for (int i = 0; i < _weapons.size(); i++) {
		Weapon *it = _weapons.at(i);

		if (!!it)
			delete it;
	}
	_weapons.clear();

	for (int i = 0; i < _accessories.size(); i++) {
		Accessory *it = _accessories.at(i);

		if (!!it)
			delete it;
	}
	_accessories.clear();
}

/**
*	Get all registered armor pieces.
*/
const QVector<Armor*>& EquipmentSystem::getArmor() const {
	return _armor;
}

/**
*	Get all registred weapons.
*/
const QVector<Weapon*>& EquipmentSystem::getWeapons() const {
	return _weapons;
}

/**
*	Get all registered accessories.
*/
const QVector<Accessory*>& EquipmentSystem::getAccessories() const {
	return _accessories;
}

/**
*	Get the armor piece with the specified name <i>name</i>.
*/
Armor* EquipmentSystem::getArmor(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for armor with an empty name.");

	return getComponent<Armor>(_armor, name);
}

/**
*	Get the weapon with the specified name <i>name</i>.
*/
Weapon* EquipmentSystem::getWeapon(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for a weapon with an empty name.");

	return getComponent<Weapon>(_weapons, name);
}

/**
*	Get the accessory with the specified name <i>name</i>.
*/
Accessory* EquipmentSystem::getAccessory(QString name) const {
	if (name == QString())
		throw ProjectException("Cannot search for an accessory with an empty name.");

	return getComponent<Accessory>(_accessories, name);
}

/**
*	Add an armor piece to the system.
*/
void EquipmentSystem::addArmor(Armor *armor) {
	if (!armor)
		throw ProjectException("Cannot add a null armor.");

	_armor.append(armor);

	Logger *logger = Logger::instance();
	logger->addMessage("Added armor " + armor->getName() + " to the project");
	logger->setStatus(true);
}

/**
*	Add a weapon to the system.
*/
void EquipmentSystem::addWeapon(Weapon *weapon) {
	if (!weapon)
		throw ProjectException("Cannot add a null weapon.");

	_weapons.append(weapon);

	Logger *logger = Logger::instance();
	logger->addMessage("Added weapon " + weapon->getName() + " to the project");
	logger->setStatus(true);
}

/**
*	Add an accessory to the system.
*/
void EquipmentSystem::addAccessory(Accessory *accessory) {
	if (!accessory)
		throw ProjectException("Cannot add a null accessory.");

	_accessories.append(accessory);

	Logger *logger = Logger::instance();
	logger->addMessage("Added accessory " + accessory->getName() + " to the project");
	logger->setStatus(true);
}

/**
*	Remove the armor piece with the specified name <i>name</i> from the system.
*/
void EquipmentSystem::removeArmor(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for armor with an empty name.");

	Armor *armor = removeComponentAndLog<Armor>(_armor, name, "armor", "project");

	if (!!armor)
		delete armor;
}

/**
*	Remove the weapon with the specified name <i>name</i> from the system.
*/
void EquipmentSystem::removeWeapon(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for a weapon with an empty name.");

	Weapon *weapon = removeComponentAndLog<Weapon>(_weapons, name, "weapon", "project");

	if (!!weapon)
		delete weapon;
}

/**
*	Remove the accessory with the specified name <i>name</i> from the system. 
*/
void EquipmentSystem::removeAccessory(QString name) {
	if (name == QString())
		throw ProjectException("Cannot search for an accessory with an empty name.");

	Accessory *accessory = removeComponentAndLog<Accessory>(_accessories, name, "accessories", "project");

	if (!!accessory)
		delete accessory;
}

/**
*	Create the equipment management system and return a pointer to it.
*/
EquipmentSystem* EquipmentSystem::create() {
	return new EquipmentSystem();
}

/**
*	Create and translate an equipment management system from XML and return a pointer to it.
*/
EquipmentSystem* EquipmentSystem::translateFromXML(QDomElement &element) {
	EquipmentSystem *system = EquipmentSystem::create();

	QDomNodeList weapons = element.firstChildElement("weapons").childNodes();
	for (int i = 0; i < weapons.size(); i++) {
		QDomElement it = weapons.at(i).toElement();
		Weapon *weapon = Weapon::translateFromXML(it);
		system->addWeapon(weapon);
	}

	QDomNodeList armors = element.firstChildElement("armor").childNodes();
	for (int i = 0; i < armors.size(); i++) {
		QDomElement it = armors.at(i).toElement();
		Armor *armor = Armor::translateFromXML(it);
		system->addArmor(armor);
	}

	QDomNodeList accessories = element.firstChildElement("accessories").childNodes();
	for (int i = 0; i < accessories.size(); i++) {
		QDomElement it = accessories.at(i).toElement();
		Accessory *accessory = Accessory::translateFromXML(it);
		system->addAccessory(accessory);
	}

	return system;
}

/**
*	Translate the equipment management system to XML and append it to the XML-document.
*/
void EquipmentSystem::translateToXML(EquipmentSystem *system, QDomDocument &document, QDomElement &parent) {
	QDomElement equipment = document.createElement("equipment");

	QVector<Weapon*> weapons = system->getWeapons();
	QDomElement weaponsElement = document.createElement("weapons");
	for (int i = 0; i < weapons.size(); i++) {
		Weapon::translateToXML(weapons.at(i), document, weaponsElement);
	}
	equipment.appendChild(weaponsElement);

	QVector<Armor*> armor = system->getArmor();
	QDomElement armorElement = document.createElement("armor");
	for (int i = 0; i < armor.size(); i++) {
		Armor::translateToXML(armor.at(i), document, armorElement);
	}
	equipment.appendChild(armorElement);

	QVector<Accessory*> accessories = system->getAccessories();
	QDomElement accessoryElement = document.createElement("accessories");
	for (int i = 0; i < accessories.size(); i++) {
		Accessory::translateToXML(accessories.at(i), document, accessoryElement);
	}
	equipment.appendChild(accessoryElement);

	parent.appendChild(equipment);
}