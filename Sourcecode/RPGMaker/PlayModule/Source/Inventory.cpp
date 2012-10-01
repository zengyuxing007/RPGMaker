#include "../Headers/Inventory.h"

using namespace playmodule;

Inventory::Inventory()
	: _currency(0) {}

int Inventory::getCurrency() const {
	return _currency;
}

QVector<QPair<Armor*, int>> Inventory::getArmor() const {
	return _armor;
}

QVector<QPair<Weapon*, int>> Inventory::getWeapons() const {
	return _weapons;
}

QVector<QPair<Accessory*, int>> Inventory::getAccessories() const {
	return _accessories;
}

QVector<QPair<EquipmentPiece*, int>> Inventory::getEquipment() const {
	QVector<QPair<EquipmentPiece*, int>> equipment(0);

	for (int i = 0; i < _armor.size(); i++) {
		QPair<Armor*, int> it = _armor.at(i);
		equipment.append(QPair<EquipmentPiece*, int>(it.first, it.second));
	}

	for (int i = 0; i < _weapons.size(); i++) {
		QPair<Weapon*, int> it = _weapons.at(i);
		equipment.append(QPair<EquipmentPiece*, int>(it.first, it.second));
	}

	for (int i = 0; i < _accessories.size(); i++) {
		QPair<Accessory*, int> it = _accessories.at(i);
		equipment.append(QPair<EquipmentPiece*, int>(it.first, it.second));
	}

	return equipment;
}

void Inventory::addCurrency(int currency) {
	_currency += currency;
}

void Inventory::addArmor(Armor *armor, int amount) {
	for (QVector<QPair<Armor*, int>>::iterator it = _armor.begin(); it != _armor.end(); it++) {
		if (it->first == armor) {
			it->second = it->second + amount;
			return;
		}
	}

	_armor.append(QPair<Armor*, int>(armor, amount));
}

void Inventory::addWeapon(Weapon *weapon, int amount) {
	for (QVector<QPair<Weapon*, int>>::iterator it = _weapons.begin(); it != _weapons.end(); it++) {
		if (it->first == weapon) {
			it->second = it->second + amount;
			return;
		}
	}

	_weapons.append(QPair<Weapon*, int>(weapon, amount));
}

void Inventory::addAccessory(Accessory *accessory, int amount) {
	for (QVector<QPair<Accessory*, int>>::iterator it = _accessories.begin(); it != _accessories.end(); it++) {
		if (it->first == accessory) {
			it->second = it->second + amount;
			return;
		}
	}

	_accessories.append(QPair<Accessory*, int>(accessory, amount));
}

void Inventory::addEquipment(EquipmentPiece *equipment, int amount) {
	switch (equipment->getType()) {
	case EquipmentPiece::ARMOR:
		addArmor(dynamic_cast<Armor*>(equipment), amount);
		break;
	case EquipmentPiece::WEAPON:
		addWeapon(dynamic_cast<Weapon*>(equipment), amount);
		break;
	case EquipmentPiece::ACCESSORY:
		addAccessory(dynamic_cast<Accessory*>(equipment), amount);
		break;
	}
}

int Inventory::takeCurrency(int currency) {
	int taken = qMin(_currency, currency);
	_currency -= taken;
	return taken;
}

void Inventory::takeArmor(QString name, int amount) {
	for (QVector<QPair<Armor*, int>>::iterator it = _armor.begin(); it != _armor.end(); it++) {
		if (it->first->getName() == name) {
			it->second = it->second - amount;

			if (it->second <= 0)
				_armor.erase(it);

			return;
		}
	}
}

void Inventory::takeWeapon(QString name, int amount) {
	for (QVector<QPair<Weapon*, int>>::iterator it = _weapons.begin(); it != _weapons.end(); it++) {
		if (it->first->getName() == name) {
			it->second = it->second - amount;

			if (it->second <= 0)
				_weapons.erase(it);

			return;
		}
	}
}

void Inventory::takeAccessory(QString name, int amount) {
	for (QVector<QPair<Accessory*, int>>::iterator it = _accessories.begin(); it != _accessories.end(); it++) {
		if (it->first->getName() == name) {
			it->second = it->second - amount;

			if (it->second <= 0)
				_accessories.erase(it);

			return;
		}
	}
}

void Inventory::takeEquipment(QString name, int amount) {
	takeArmor(name, amount);
	takeWeapon(name, amount);
	takeAccessory(name, amount);
}