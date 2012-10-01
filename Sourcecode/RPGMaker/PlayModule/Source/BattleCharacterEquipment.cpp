#include "../Headers/BattleCharacterEquipment.h"

using namespace playmodule;

BattleCharacterEquipment::BattleCharacterEquipment()
	: _weapons(QPair<Weapon*, Weapon*>(0, 0)),
	_armor(QVector<Armor*>(5, 0)),
	_accessories(QVector<Accessory*>(3, 0)) {}

BattleCharacterEquipment::~BattleCharacterEquipment() {}

QVector<EquipmentPiece*> BattleCharacterEquipment::getEquipment() const {
	QVector<EquipmentPiece*> equipment(0);

	equipment.append(_weapons.first);
	equipment.append(_weapons.second);

	for (QVector<Armor*>::const_iterator it = _armor.begin(); it != _armor.end(); it++)
		equipment.append(*it);

	for (QVector<Accessory*>::const_iterator it = _accessories.begin(); it != _accessories.end(); it++)
		equipment.append(*it);

	return equipment;
}

QPair<Weapon*, Weapon*> BattleCharacterEquipment::getWeapons() const {
	return _weapons;
}

QVector<Armor*> BattleCharacterEquipment::getArmor() const {
	return _armor;
}

QVector<Accessory*> BattleCharacterEquipment::getAccessories() const {
	return _accessories;
}

Weapon* BattleCharacterEquipment::getMainhand() const {
	if (!!_weapons.first && (_weapons.first->getSubtype() == Weapon::MAINHAND))
		return _weapons.first;
	else
		return 0;
}

Weapon* BattleCharacterEquipment::getOffhand() const {
	if (!!_weapons.second && (_weapons.second->getSubtype() == Weapon::OFFHAND))
		return _weapons.second;
	else
		return 0;
}

Weapon* BattleCharacterEquipment::getTwohanded() const {
	if (!!_weapons.first && !!_weapons.second && (_weapons.first->getSubtype() == Weapon::TWOHANDED) && (_weapons.second->getSubtype() == Weapon::TWOHANDED) && (_weapons.first == _weapons.second))
		return _weapons.first;
	else
		return 0;
}

Armor* BattleCharacterEquipment::getHelment() const {
	return _armor.at(0);
}

Armor* BattleCharacterEquipment::getTorso() const {
	return _armor.at(1);
}

Armor* BattleCharacterEquipment::getLeggings() const {
	return _armor.at(2);
}

Armor* BattleCharacterEquipment::getGloves() const {
	return _armor.at(3);
}

Armor* BattleCharacterEquipment::getBoots() const {
	return _armor.at(4);
}

Accessory* BattleCharacterEquipment::getEarring() const {
	return _accessories.at(0);
}

Accessory* BattleCharacterEquipment::getNecklace() const {
	return _accessories.at(1);
}

Accessory* BattleCharacterEquipment::getRing() const {
	return _accessories.at(2);
}

void BattleCharacterEquipment::setWeapon(Weapon *weapon) {
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
	}
}

void BattleCharacterEquipment::setArmor(Armor *armor) {
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
	}
}

void BattleCharacterEquipment::setAccessory(Accessory *accessory) {
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
	}
}

void BattleCharacterEquipment::setMainhand(Weapon *weapon) {
	if (!!_weapons.second && (_weapons.second->getSubtype() == Weapon::MAINHAND))
		_weapons.second = 0;

	_weapons.first = weapon;
}

void BattleCharacterEquipment::setOffhand(Weapon *weapon) {
	if (!!_weapons.first && (_weapons.first->getSubtype() == Weapon::MAINHAND))
		_weapons.first = 0;

	_weapons.second = weapon;
}

void BattleCharacterEquipment::setTwohanded(Weapon *weapon) {
	_weapons.first = weapon;
	_weapons.second = weapon;
}

void BattleCharacterEquipment::setHelmet(Armor *helmet) {
	_armor.replace(0, helmet);
}

void BattleCharacterEquipment::setTorso(Armor *torso) {
	_armor.replace(1, torso);
}

void BattleCharacterEquipment::setLeggings(Armor *leggings) {
	_armor.replace(2, leggings);
}

void BattleCharacterEquipment::setGloves(Armor *gloves) {
	_armor.replace(3, gloves);
}

void BattleCharacterEquipment::setBoots(Armor *boots) {
	_armor.replace(4, boots);
}

void BattleCharacterEquipment::setEarring(Accessory *earring) {
	_accessories.replace(0, earring);
}

void BattleCharacterEquipment::setNecklace(Accessory *necklace) {
	_accessories.replace(1, necklace);
}

void BattleCharacterEquipment::setRing(Accessory *ring) {
	_accessories.replace(2, ring);
}

void BattleCharacterEquipment::setEquipment(EquipmentPiece *equipment) {
	switch (equipment->getType()) {
	case EquipmentPiece::WEAPON:
		setWeapon(dynamic_cast<Weapon*>(equipment));
		break;
	case EquipmentPiece::ARMOR:
		setArmor(dynamic_cast<Armor*>(equipment));
		break;
	case EquipmentPiece::ACCESSORY:
		setAccessory(dynamic_cast<Accessory*>(equipment));
		break;
	}
}

void BattleCharacterEquipment::removeEquipment(EquipmentPiece *equipment) {
	switch (equipment->getType()) {
	case EquipmentPiece::WEAPON:
		{
			Weapon *weapon = dynamic_cast<Weapon*>(equipment);
			switch (weapon->getSubtype()) {
			case Weapon::MAINHAND:
				setMainhand(0);
				break;
			case Weapon::OFFHAND:
				setOffhand(0);
				break;
			case Weapon::TWOHANDED:
				setTwohanded(0);
				break;
			}
		}
		break;
	case EquipmentPiece::ARMOR:
		{
			Armor *armor = dynamic_cast<Armor*>(equipment);
			switch (armor->getSubtype()) {
			case Armor::HELMET:
				setHelmet(0);
				break;
			case Armor::TORSO:
				setTorso(0);
				break;
			case Armor::LEGGINGS:
				setLeggings(0);
				break;
			case Armor::GLOVES:
				setGloves(0);
				break;
			case Armor::BOOTS:
				setBoots(0);
				break;
			}
		}
		break;
	case EquipmentPiece::ACCESSORY:
		{
			Accessory *accessory = dynamic_cast<Accessory*>(equipment);
			switch (accessory->getSubtype()) {
			case Accessory::EARRING:
				setEarring(0);
				break;
			case Accessory::NECKLACE:
				setNecklace(0);
				break;
			case Accessory::RING:
				setRing(0);
				break;
			}
		}
		break;
	}
}