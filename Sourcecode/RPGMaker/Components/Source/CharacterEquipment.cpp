#include "../Logger.h"
#include "../Headers/CharacterEquipment.h"

using namespace projectcomponents;

CharacterEquipment::CharacterEquipment()
	: _weapons(QPair<Weapon*, Weapon*>(0, 0)),
	_armor(QVector<Armor*>(5, 0)),
	_accessories(QVector<Accessory*>(3, 0)) {}

CharacterEquipment::~CharacterEquipment() {}

QVector<EquipmentPiece*> CharacterEquipment::getEquipment() const {
	QVector<EquipmentPiece*> equipment(0);

	equipment.append(_weapons.first);
	equipment.append(_weapons.second);

	for (QVector<Armor*>::const_iterator it = _armor.begin(); it != _armor.end(); it++)
		equipment.append(*it);

	for (QVector<Accessory*>::const_iterator it = _accessories.begin(); it != _accessories.end(); it++)
		equipment.append(*it);

	return equipment;
}

QPair<Weapon*, Weapon*> CharacterEquipment::getWeapons() const {
	return _weapons;
}

QVector<Armor*> CharacterEquipment::getArmor() const {
	return _armor;
}

QVector<Accessory*> CharacterEquipment::getAccessories() const {
	return _accessories;
}

Weapon* CharacterEquipment::getMainhand() const {
	if (!!_weapons.first && (_weapons.first->getSubtype() == Weapon::MAINHAND))
		return _weapons.first;
	else
		return 0;
}

Weapon* CharacterEquipment::getOffhand() const {
	if (!!_weapons.second && (_weapons.second->getSubtype() == Weapon::OFFHAND))
		return _weapons.second;
	else
		return 0;
}

Weapon* CharacterEquipment::getTwohanded() const {
	if (!!_weapons.first && !!_weapons.second && (_weapons.first->getSubtype() == Weapon::TWOHANDED) && (_weapons.second->getSubtype() == Weapon::TWOHANDED) && (_weapons.first == _weapons.second))
		return _weapons.first;
	else
		return 0;
}

Armor* CharacterEquipment::getHelment() const {
	return _armor.at(0);
}

Armor* CharacterEquipment::getTorso() const {
	return _armor.at(1);
}

Armor* CharacterEquipment::getLeggings() const {
	return _armor.at(2);
}

Armor* CharacterEquipment::getGloves() const {
	return _armor.at(3);
}

Armor* CharacterEquipment::getBoots() const {
	return _armor.at(4);
}

Accessory* CharacterEquipment::getEarring() const {
	return _accessories.at(0);
}

Accessory* CharacterEquipment::getNecklace() const {
	return _accessories.at(1);
}

Accessory* CharacterEquipment::getRing() const {
	return _accessories.at(2);
}

void CharacterEquipment::setWeapon(Weapon *weapon) {
	if (!weapon)
		throw ProjectException("A null weapon cannot be set. Use the appropriate method to do so.");

	switch(weapon->getSubtype()) {
	case Weapon::MAINHAND:
		setMainhand(weapon);
		break;
	case Weapon::OFFHAND:
		setOffhand(weapon);
		break;
	case Weapon::TWOHANDED:
		setTwohanded(weapon);
		break;
	default:
		throw ProjectException("Unknown weapon type.");
	}
}

void CharacterEquipment::setArmor(Armor *armor) {
	if (!armor)
		throw ProjectException("A null armor cannot be set. Use the appropriate method to do so.");

	switch (armor->getSubtype()) {
	case Armor::HELMET:
		setHelmet(armor);
		break;
	case  Armor::TORSO:
		setTorso(armor);
		break;
	case Armor::LEGGINGS:
		setLeggings(armor);
		break;
	case Armor::GLOVES:
		setGloves(armor);
		break;
	case Armor::BOOTS:
		setBoots(armor);
		break;
	default:
		throw ProjectException("Unknown armor type.");
	}
}

void CharacterEquipment::setAccessory(Accessory *accessory) {
	if (!accessory)
		throw ProjectException("A null accessory cannot be set. Use the appropriate method to do so.");

	switch (accessory->getSubtype()) {
	case Accessory::EARRING:
		setEarring(accessory);
		break;
	case Accessory::NECKLACE:
		setNecklace(accessory);
		break;
	case Accessory::RING:
		setRing(accessory);
		break;
	default:
		throw ProjectException("Unknown accessory type.");
	}
}

void CharacterEquipment::setMainhand(Weapon *weapon) {
	if (!weapon)
		throw ProjectException("A null weapon cannot be set. Use the appropriate method to do so.");

	if (weapon->getSubtype() != Weapon::MAINHAND)
		throw ProjectException("The type of the weapon does not match.");
	
	if (!!_weapons.first && (_weapons.first->getSubtype() == Weapon::TWOHANDED)) {
		unsetTwohanded();
	}

	_weapons.first = weapon;
	connect(weapon, SIGNAL(deleteSignal(QString)), this, SLOT(deleteWeapon(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed mainhand weapon");
	logger->setStatus(true);
}

void CharacterEquipment::setOffhand(Weapon *weapon) {
	if (!weapon)
		throw ProjectException("A null weapon cannot be set. Use the appropriate method to do so.");

	if (weapon->getSubtype() != Weapon::OFFHAND)
		throw ProjectException("The type of the weapon does not match.");

	if (!!_weapons.second && (_weapons.second->getSubtype() == Weapon::TWOHANDED)) {
		unsetTwohanded();
	}

	_weapons.second = weapon;
	connect(weapon, SIGNAL(deleteSignal(QString)), this, SLOT(deleteWeapon(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed offhand weapon");
	logger->setStatus(true);
}

void CharacterEquipment::setTwohanded(Weapon *weapon) {
	if (!weapon)
		throw ProjectException("A null weapon cannot be set. Use the appropriate method to do so.");

	if (weapon->getSubtype() != Weapon::TWOHANDED)
		throw ProjectException("The type of the weapon does not match.");

	if (!!_weapons.first && (_weapons.first->getSubtype() == Weapon::TWOHANDED)) {
		unsetTwohanded();
	} else {
		if (!!_weapons.first)
			disconnect(_weapons.first, 0, this, 0);

		if (!!_weapons.second)
			disconnect(_weapons.second, 0, this, 0);
	}

	_weapons.first = weapon;
	_weapons.second = weapon;
	connect(weapon, SIGNAL(deleteSignal(QString)), this, SLOT(deleteWeapon(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed twohanded weapon");
	logger->setStatus(true);
}

void CharacterEquipment::setHelmet(Armor *helmet) {
	if (!helmet)
		throw ProjectException("A null armor cannot be set. Use the appropriate method to do so.");

	if (helmet->getSubtype() != Armor::HELMET)
		throw ProjectException("The type of the armor does not match.");

	if (!!_armor.at(0))
		disconnect(_armor.at(0), 0, this, 0);

	_armor.replace(0, helmet);
	connect(helmet, SIGNAL(deleteSignal(QString)), this, SLOT(deleteArmor(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed helmet");
	logger->setStatus(true);
}

void CharacterEquipment::setTorso(Armor *torso) {
	if (!torso)
		throw ProjectException("A null armor cannot be set. Use the appropriate method to do so.");

	if (torso->getSubtype() != Armor::TORSO)
		throw ProjectException("The type of the armor does not match.");

	if (!!_armor.at(1))
		disconnect(_armor.at(1), 0, this, 0);

	_armor.replace(1, torso);
	connect(torso, SIGNAL(deleteSignal(QString)), this, SLOT(deleteArmor(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed torso");
	logger->setStatus(true);
}

void CharacterEquipment::setLeggings(Armor *leggings) {
	if (!leggings)
		throw ProjectException("A null armor cannot be set. Use the appropriate method to do so.");

	if (leggings->getSubtype() != Armor::LEGGINGS)
		throw ProjectException("The type of the armor does not match.");

	if (!!_armor.at(2))
		disconnect(_armor.at(2), 0, this, 0);

	_armor.replace(2, leggings);
	connect(leggings, SIGNAL(deleteSignal(QString)), this, SLOT(deleteArmor(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed leggings");
	logger->setStatus(true);
}

void CharacterEquipment::setGloves(Armor *gloves) {
	if (!gloves)
		throw ProjectException("A null armor cannot be set. Use the appropriate method to do so.");

	if (gloves->getSubtype() != Armor::GLOVES)
		throw ProjectException("The type of the armor does not match.");

	if (!!_armor.at(3))
		disconnect(_armor.at(3), 0, this, 0);

	_armor.replace(3, gloves);
	connect(gloves, SIGNAL(deleteSignal(QString)), this, SLOT(deleteArmor(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed gloves");
	logger->setStatus(true);
}

void CharacterEquipment::setBoots(Armor *boots) {
	if (!boots)
		throw ProjectException("A null armor cannot be set. Use the appropriate method to do so.");

	if (boots->getSubtype() != Armor::BOOTS)
		throw ProjectException("The type of the armor does not match.");

	if (!!_armor.at(4))
		disconnect(_armor.at(4), 0, this, 0);

	_armor.replace(4, boots);
	connect(boots, SIGNAL(deleteSignal(QString)), this, SLOT(deleteArmor(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed boots");
	logger->setStatus(true);
}

void CharacterEquipment::setEarring(Accessory *earring) {
	if (!earring)
		throw ProjectException("A null accessory cannot be set. Use the appropriate method to do so.");

	if (earring->getSubtype() != Accessory::EARRING)
		throw ProjectException("The type of the accessory does not match.");

	if (!!_accessories.at(0))
		disconnect(_accessories.at(0), 0, this, 0);

	_accessories.replace(0, earring);
	connect(earring, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAccessory(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed earring");
	logger->setStatus(true);
}

void CharacterEquipment::setNecklace(Accessory *necklace) {
	if (!necklace)
		throw ProjectException("A null accessory cannot be set. Use the appropriate method to do so.");

	if (necklace->getSubtype() != Accessory::NECKLACE)
		throw ProjectException("The type of the accessory does not match.");

	if (!!_accessories.at(1))
		disconnect(_accessories.at(1), 0, this, 0);

	_accessories.replace(1, necklace);
	connect(necklace, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAccessory(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed necklace");
	logger->setStatus(true);
}

void CharacterEquipment::setRing(Accessory *ring) {
	if (!ring)
		throw ProjectException("A null accessory cannot be set. Use the appropriate method to do so.");

	if (ring->getSubtype() != Accessory::RING)
		throw ProjectException("The type of the accessory does not match.");

	if (!!_accessories.at(2))
		disconnect(_accessories.at(2), 0, this, 0);

	_accessories.replace(2, ring);
	connect(ring, SIGNAL(deleteSignal(QString)), this, SLOT(deleteAccessory(QString)));

	Logger *logger = Logger::instance();
	logger->addMessage("Placed ring");
	logger->setStatus(true);
}

void CharacterEquipment::unsetMainhand() {
	if (!_weapons.first)
		throw ProjectException("There is no weapon placed in the slot.");

	if (!!_weapons.first && (_weapons.first->getSubtype() != Weapon::MAINHAND))
		throw ProjectException("The type of the weapon does not match.");

	disconnect(_weapons.first, 0, this, 0);
	_weapons.first = 0;

	Logger *logger = Logger::instance();
	logger->addMessage("Removed mainhand");
	logger->setStatus(true);
}

void CharacterEquipment::unsetOffhand() {
	if (!_weapons.second)
		throw ProjectException("There is no weapon placed in the slot.");

	if (!!_weapons.second && (_weapons.second->getSubtype() != Weapon::OFFHAND))
		throw ProjectException("The type of the weapon does not match.");

	disconnect(_weapons.second, 0, this, 0);
	_weapons.second = 0;

	Logger *logger = Logger::instance();
	logger->addMessage("Removed offhand");
	logger->setStatus(true);
}

void CharacterEquipment::unsetTwohanded() {
	if (!_weapons.first)
		throw ProjectException("There is no weapon placed in the slot.");

	if (!_weapons.second)
		throw ProjectException("There is no weapon placed in the slot.");

	if (_weapons.first->getSubtype() != Weapon::TWOHANDED)
		throw ProjectException("The type of the weapon does not match.");

	if (_weapons.second->getSubtype() != Weapon::TWOHANDED)
		throw ProjectException("The type of the weapon does not match.");

	disconnect(_weapons.first, 0, this, 0);
	_weapons.first = 0;
	_weapons.second = 0;

	Logger *logger = Logger::instance();
	logger->addMessage("Removed twohanded");
	logger->setStatus(true);
}

void CharacterEquipment::unsetHelmet() {
	if (!_armor.at(0))
		throw ProjectException("There is no armor placed in the slot.");

	disconnect(_armor.at(0), 0, this, 0);
	_armor.replace(0, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed helmet");
	logger->setStatus(true);
}

void CharacterEquipment::unsetTorso() {
	if (!_armor.at(1))
		throw ProjectException("There is no armor placed in the slot.");

	disconnect(_armor.at(1), 0, this, 0);
	_armor.replace(1, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed torso");
	logger->setStatus(true);
}

void CharacterEquipment::unsetLeggings() {
	if (!_armor.at(2))
		throw ProjectException("There is no armor placed in the slot.");

	disconnect(_armor.at(2), 0, this, 0);
	_armor.replace(2, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed leggings");
	logger->setStatus(true);
}

void CharacterEquipment::unsetGloves() {
	if (!_armor.at(3))
		throw ProjectException("There is no armor placed in the slot.");

	disconnect(_armor.at(3), 0, this, 0);
	_armor.replace(3, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed gloves");
	logger->setStatus(true);
}

void CharacterEquipment::unsetBoots() {
	if (!_armor.at(4))
		throw ProjectException("There is no armor placed in the slot.");

	disconnect(_armor.at(4), 0, this, 0);
	_armor.replace(4, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed boots");
	logger->setStatus(true);
}

void CharacterEquipment::unsetEarring() {
	if (!_accessories.at(0))
		throw ProjectException("There is no accessory placed in the slot.");

	disconnect(_accessories.at(0), 0, this, 0);
	_accessories.replace(0, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed earring");
	logger->setStatus(true);
}

void CharacterEquipment::unsetNecklace() {
	if (!_accessories.at(1))
		throw ProjectException("There is no accessory placed in the slot.");

	disconnect(_accessories.at(1), 0, this, 0);
	_accessories.replace(1, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed necklace");
	logger->setStatus(true);
}

void CharacterEquipment::unsetRing() {
	if (!_accessories.at(2))
		throw ProjectException("There is no accessory placed in the slot.");

	disconnect(_accessories.at(2), 0, this, 0);
	_accessories.replace(2, 0);

	Logger *logger = Logger::instance();
	logger->addMessage("Removed ring");
	logger->setStatus(true);
}

CharacterEquipment* CharacterEquipment::create() {
	return new CharacterEquipment();
}

void CharacterEquipment::translateToXML(CharacterEquipment *equipment, QDomDocument &document, QDomElement &parent) {
	QDomElement weaponsElement = document.createElement("weapons");
	if (!!equipment->_weapons.first) {
		QDomElement weaponElement = document.createElement("weapon");
		weaponElement.setAttribute("name", equipment->_weapons.first->getName());
		weaponsElement.appendChild(weaponElement);
	}

	if (!!equipment->_weapons.second && (equipment->_weapons.second->getSubtype() == Weapon::OFFHAND)) {
		QDomElement weaponElement = document.createElement("weapon");
		weaponElement.setAttribute("name", equipment->_weapons.second->getName());
		weaponsElement.appendChild(weaponElement);
	}
	parent.appendChild(weaponsElement);

	QDomElement armorsElement = document.createElement("armor");
	if (!!equipment->_armor.at(0)) {
		QDomElement armorElement = document.createElement("helmet");
		armorElement.setAttribute("name", equipment->_armor.at(0)->getName());
		armorsElement.appendChild(armorElement);
	}
	if (!!equipment->_armor.at(1)) {
		QDomElement armorElement = document.createElement("torso");
		armorElement.setAttribute("name", equipment->_armor.at(1)->getName());
		armorsElement.appendChild(armorElement);
	}
	if (!!equipment->_armor.at(2)) {
		QDomElement armorElement = document.createElement("leggings");
		armorElement.setAttribute("name", equipment->_armor.at(2)->getName());
		armorsElement.appendChild(armorElement);
	}
	if (!!equipment->_armor.at(3)) {
		QDomElement armorElement = document.createElement("gloves");
		armorElement.setAttribute("name", equipment->_armor.at(3)->getName());
		armorsElement.appendChild(armorElement);
	}
	if (!!equipment->_armor.at(4)) {
		QDomElement armorElement = document.createElement("boots");
		armorElement.setAttribute("name", equipment->_armor.at(4)->getName());
		armorsElement.appendChild(armorElement);
	}
	parent.appendChild(armorsElement);

	QDomElement accessoriesElement = document.createElement("accessories");
	if (!!equipment->_accessories.at(0)) {
		QDomElement accessoryElement = document.createElement("earring");
		accessoryElement.setAttribute("name", equipment->_accessories.at(0)->getName());
		accessoriesElement.appendChild(accessoryElement);
	}
	if (!!equipment->_accessories.at(1)) {
		QDomElement accessoryElement = document.createElement("necklace");
		accessoryElement.setAttribute("name", equipment->_accessories.at(1)->getName());
		accessoriesElement.appendChild(accessoryElement);
	}
	if (!!equipment->_accessories.at(2)) {
		QDomElement accessoryElement = document.createElement("ring");
		accessoryElement.setAttribute("name", equipment->_accessories.at(2)->getName());
		accessoriesElement.appendChild(accessoryElement);
	}
	parent.appendChild(accessoriesElement);
}

void CharacterEquipment::deleteWeapon(QString name) {
	if (!!_weapons.first && (_weapons.first->getName() == name))
		_weapons.first = 0;

	if (!!_weapons.second && (_weapons.second->getName() == name))
		_weapons.second = 0;
}

void CharacterEquipment::deleteArmor(QString name) {
	for (int i = 0; i < _armor.size(); i++) {
		Armor *it = _armor.at(i);
		if (!!it && it->getName() == name)
			_armor.replace(i, 0);
	}
}

void CharacterEquipment::deleteAccessory(QString name) {
	for (int i = 0; i < _accessories.size(); i++) {
		Accessory *it = _accessories.at(i);
		if (!!it && it->getName() == name)
			_accessories.replace(i, 0);
	}
}