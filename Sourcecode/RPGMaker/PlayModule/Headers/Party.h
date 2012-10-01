#ifndef PARTY_H
#define PARTY_H

#include "qvector.h"

#include "Inventory.h"
#include "../Headers/PlayException.h"
#include "../Headers/BattleCharacterItem.h"
#include "../Components/Headers/Character.h"

using projectcomponents::Character;

namespace playmodule {

	class Party {
	public:
		Party();
		Party(QVector<Character*> characters);
		virtual ~Party();

		Inventory* getInventory() const;

		BattleCharacterItem* getPartyLeader() const;
		BattleCharacterItem* getPartyMember(int position) const throw (PlayException);
		BattleCharacterItem* getPartyMember(QString name) const throw (PlayException);
		QVector<BattleCharacterItem*> getPartyMembers() const;

		void setPartyLeader(BattleCharacterItem *character) throw (PlayException);
		void setPartyMember(int position, BattleCharacterItem *character) throw (PlayException);
		void switchPlace(int position1, int position2) throw (PlayException);
		void switchPlace(QString name1, QString name2) throw (PlayException);

		void unsetPartyLeader() throw (PlayException);
		void unsetPartyMember(int position) throw (PlayException);
		void unsetPartyMember(QString name) throw (PlayException);

	private:
		Inventory *_inventory;
		QVector<BattleCharacterItem*> _characters;
	};
}

#endif