#ifndef BATTLECHARACTEREQUIPMENT_H
#define BATTLECHARACTEREQUIPMENT_H

#include "qpair.h"
#include "qvector.h"

#include "../Components/Headers/Armor.h"
#include "../Components/Headers/Weapon.h"
#include "../Components/Headers/Accessory.h"

using projectcomponents::Armor;
using projectcomponents::Weapon;
using projectcomponents::Accessory;

namespace playmodule {

	class BattleCharacterEquipment {
	public:

		BattleCharacterEquipment();
		virtual ~BattleCharacterEquipment();

		QVector<EquipmentPiece*> getEquipment() const;

		QPair<Weapon*, Weapon*> getWeapons() const;
		QVector<Armor*> getArmor() const;
		QVector<Accessory*> getAccessories() const;

		Weapon* getMainhand() const;
		Weapon* getOffhand() const;
		Weapon* getTwohanded() const;

		Armor* getHelment() const;
		Armor* getTorso() const;
		Armor* getLeggings() const;
		Armor* getGloves() const;
		Armor* getBoots() const;

		Accessory* getEarring() const;
		Accessory* getNecklace() const;
		Accessory* getRing() const;

		void setWeapon(Weapon *weapon);
		void setArmor(Armor *armor);
		void setAccessory(Accessory *accessory);

		void setMainhand(Weapon *weapon);
		void setOffhand(Weapon *weapon);
		void setTwohanded(Weapon *weapon);

		void setHelmet(Armor *helmet);
		void setTorso(Armor *torso);
		void setLeggings(Armor *leggings);
		void setGloves(Armor *gloves);
		void setBoots(Armor *boots);

		void setEarring(Accessory *earring);
		void setNecklace(Accessory *necklace);
		void setRing(Accessory *ring);

		void setEquipment(EquipmentPiece *equipment);
		void removeEquipment(EquipmentPiece *equipment);

	private:
		QPair<Weapon*, Weapon*> _weapons;
		QVector<Armor*> _armor;
		QVector<Accessory*> _accessories;
	};
}

#endif