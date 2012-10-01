#ifndef CHARACTEREQUIPMENT_H
#define CHARACTEREQUIPMENT_H

#include "qdom.h"
#include "qpair.h"
#include "qvector.h"
#include "qobject.h"

#include "Armor.h"
#include "Weapon.h"
#include "Accessory.h"
#include "ProjectException.h"

using namespace projectcomponents;

namespace projectcomponents {

	/**
	*	The CharacterEquipment class is a management system for the equipment of the character it belongs to.
	*	Every type of equipment has a certain amount of slots in which equipment of the correct type can be placed.
	*/
	class CharacterEquipment : public QObject {

		Q_OBJECT

	public:
		virtual ~CharacterEquipment();

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

		void setWeapon(Weapon *weapon) throw (ProjectException);
		void setArmor(Armor *armor) throw (ProjectException);
		void setAccessory(Accessory *accessory) throw(ProjectException);

		void setMainhand(Weapon *weapon) throw (ProjectException);
		void setOffhand(Weapon *weapon) throw (ProjectException);
		void setTwohanded(Weapon *weapon) throw (ProjectException);

		void setHelmet(Armor *helmet) throw (ProjectException);
		void setTorso(Armor *torso) throw (ProjectException);
		void setLeggings(Armor *leggings) throw (ProjectException);
		void setGloves(Armor *gloves) throw (ProjectException);
		void setBoots(Armor *boots) throw (ProjectException);

		void setEarring(Accessory *earring) throw (ProjectException);
		void setNecklace(Accessory *necklace) throw (ProjectException);
		void setRing(Accessory *ring) throw (ProjectException);

		void unsetMainhand() throw (ProjectException);
		void unsetOffhand() throw (ProjectException);
		void unsetTwohanded() throw (ProjectException);

		void unsetHelmet() throw (ProjectException);
		void unsetTorso() throw (ProjectException);
		void unsetLeggings() throw (ProjectException);
		void unsetGloves() throw (ProjectException);
		void unsetBoots() throw (ProjectException);

		void unsetEarring() throw (ProjectException);
		void unsetNecklace() throw (ProjectException);
		void unsetRing() throw (ProjectException);

		static CharacterEquipment* create() throw (ProjectException);
		static void translateToXML(CharacterEquipment *equipment, QDomDocument &document, QDomElement &parent);

	private:
		CharacterEquipment();

		QPair<Weapon*, Weapon*> _weapons;
		QVector<Armor*> _armor;
		QVector<Accessory*> _accessories;

	private slots:
		void deleteWeapon(QString name);
		void deleteArmor(QString name);
		void deleteAccessory(QString name);
	};
}

#endif