#ifndef EQUIPMENTSYSTEM_H
#define EQUIPMENTSYSTEM_H

#include "qobject.h"
#include "qvector.h"

#include "Armor.h"
#include "Weapon.h"
#include "Accessory.h"

namespace projectcomponents {

	/**
	*	The EquipmentSystem class is a managemetsystem for the <i>Weapon</i>, <i>Armor</i> and <i>Accessory</i> classes.
	*/
	class EquipmentSystem : public QObject {

		Q_OBJECT

	public:
		virtual ~EquipmentSystem();

		const QVector<Armor*>& getArmor() const;
		const QVector<Weapon*>& getWeapons() const;
		const QVector<Accessory*>& getAccessories() const;

		Armor* getArmor(QString name) const throw (ProjectException);
		Weapon* getWeapon(QString name) const throw (ProjectException);
		Accessory* getAccessory(QString name) const throw (ProjectException);

		void addArmor(Armor *armor) throw (ProjectException);
		void addWeapon(Weapon *weapon) throw (ProjectException);
		void addAccessory(Accessory *accessory) throw (ProjectException);

		void removeArmor(QString name) throw (ProjectException);
		void removeWeapon(QString name) throw (ProjectException);
		void removeAccessory(QString name) throw (ProjectException);

		static EquipmentSystem* create();
		static EquipmentSystem* translateFromXML(QDomElement &element) throw (ProjectException);
		static void translateToXML(EquipmentSystem *system, QDomDocument &document, QDomElement &parent);

	private:
		EquipmentSystem();

		QVector<Armor*> _armor;
		QVector<Weapon*> _weapons;
		QVector<Accessory*> _accessories;
	};
}

#endif