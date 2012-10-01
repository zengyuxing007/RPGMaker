#ifndef BATTLECHARACTERITEM_H
#define BATTLECHARACTERITEM_H

#include "BattleItem.h"
#include "BattleCharacterEquipment.h"
#include "../Components/Headers/Character.h"

using projectcomponents::Character;

namespace playmodule {

	class BattleCharacterItem : public BattleItem {

		Q_OBJECT

	public:
		
		BattleCharacterItem(QGraphicsItem *parent);
		virtual ~BattleCharacterItem();

		virtual QString getName() const;
		Character* getCharacter() const;
		int getRequiredExperience() const;
		BattleCharacterEquipment *getEquipment() const;

		void initialize(Character *character);
		void addExperience(int experience);
		void recalibrate();

	private:
		Character *_character;
		BattleCharacterEquipment *_equipment;
	};
}

#endif