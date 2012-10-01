#ifndef BATTLEAI_H
#define BATTLEAI_H

#include "qpair.h"
#include "qvector.h"

#include "BattleAIBattlerInfo.h"
#include "../PlayModule/Headers/BattleAction.h"
#include "../PlayModule/Headers/BattleEnemyItem.h"
#include "../PlayModule/Headers/BattleCharacterItem.h"

using playmodule::BattleAction;
using playmodule::BattleEnemyItem;
using playmodule::BattleCharacterItem;

namespace aimodule {

	/**
	*	The battle AI takes control over the player's opponents.
	*	It will try to keep his team healthy if possible in the first play.
	*	If the party is healthy, or if the AI is unable to heal the party, it will attack the player.
	*/
	class BattleAI {
	public:

		BattleAI();
		virtual ~BattleAI();

		void initialize(QVector<BattleItem*> opponents, QVector<BattleItem*> battlers, BattleAction *turn);
		void opponentKO(BattleItem *opponent);
		void battlerKO(BattleItem *battler);

		void search();
		void updateOffensiveInfo();
		void updateDefensiveInfo();

	private:
		void searchOffensiveAction();
		void searchDefensiveAction();

		int attackExpectation(QPair<BattleItem*, BattleAIBattlerInfo*> target);
		int offensiveAbilityExpectation(QPair<BattleItem*, BattleAIBattlerInfo*> target, Ability *ability);
		int offensiveActionExpectation(QPair<BattleItem*, BattleAIBattlerInfo*> target, QVector<Element*> elements, QPair<Attribute*, int> attribute);
		int defensiveAbilityExpectation(BattleItem *target, Ability *ability);
		int defensiveActionExcpectation(BattleItem *target, QVector<Element*> elements, QPair<Attribute*, int> attribute);

		BattleAction *_turn;
		QVector<QPair<BattleItem*, BattleAIBattlerInfo*>> _opponents;
		QVector<QPair<BattleItem*, BattleAIBattlerInfo*>> _activeOpponents;
		QVector<BattleItem*> _battlers;
		QVector<BattleItem*> _activeBattlers;
	};
}
#endif