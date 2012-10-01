#ifndef INVENTORY_H
#define INVENTORY_H

#include "qpair.h"
#include "qstring.h"
#include "qvector.h"

#include "../Components/Headers/Armor.h"
#include "../Components/Headers/Weapon.h"
#include "../Components/Headers/Accessory.h"
#include "../Components/Headers/EquipmentPiece.h"

namespace playmodule {

	class Inventory {
	public:

		Inventory();

		int getCurrency() const;

		QVector<QPair<Armor*, int>> getArmor() const;
		QVector<QPair<Weapon*, int>> getWeapons() const;
		QVector<QPair<Accessory*, int>> getAccessories() const;
		QVector<QPair<EquipmentPiece*, int>> getEquipment() const;

		void addCurrency(int currency);

		void addArmor(Armor *armor, int amount);
		void addWeapon(Weapon *weapon, int amount);
		void addAccessory(Accessory *accessory, int amount);
		void addEquipment(EquipmentPiece *equipment, int amount);

		int takeCurrency(int currency);

		void takeArmor(QString name, int amount);
		void takeWeapon(QString name, int amount);
		void takeAccessory(QString name, int amount);
		void takeEquipment(QString name, int amount);

	private:
		int _currency;

		QVector<QPair<Armor*, int>> _armor;
		QVector<QPair<Weapon*, int>> _weapons;
		QVector<QPair<Accessory*, int>> _accessories;
	};
}

#endif