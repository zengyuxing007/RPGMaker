#ifndef BATTLEACTION_H
#define BATTLEACTION_H

#include "qpair.h"

#include "BattleItem.h"
#include "../Components/Headers/Ability.h"

using projectcomponents::Ability;

namespace playmodule {

	class BattleAction {
	public:

		enum Action {ATTACK, ABILITY, FLEE};

		BattleAction();
		virtual ~BattleAction();

		BattleItem* getBattler() const;
		BattleItem* getTarget() const;
		BattleAction::Action getAction() const;
		Ability* getAbility() const;

		void setBattler(BattleItem *battler);
		void setTarget(BattleItem *target);
		void setAction(BattleAction::Action action);
		void setAbility(Ability *ability);

	private:
		BattleItem *_battler;
		BattleItem *_target;
		BattleAction::Action _action;
		Ability *_ability;
	};
}

#endif