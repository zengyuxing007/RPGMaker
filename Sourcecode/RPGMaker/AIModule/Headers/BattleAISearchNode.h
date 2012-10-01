#ifndef BATTLEAISEARCHNODE_H
#define BATTLEAISEARCHNODE_H

#include "../Components/Headers/Ability.h"
#include "../PlayModule/Headers/BattleItem.h"
#include "../PlayModule/Headers/BattleAction.h"

using playmodule::BattleItem;
using playmodule::BattleAction;
using projectcomponents::Ability;

namespace aimodule {

	/**
	*	A search node for the battle AI contains a target, an action and an expected value.
	*	These values are used in the search for the best action the Battle AI can do.
	*/
	class BattleAISearchNode {
	public:

		BattleAISearchNode();
		virtual ~BattleAISearchNode();

		BattleItem* getTarget() const;
		BattleAction::Action getAction() const;
		Ability* getAbility() const;
		int getExpectedValue() const;

		void setTarget(BattleItem *target);
		void setAction(BattleAction::Action action);
		void setAbility(Ability *ability);
		void setExpectedValue(int value);


	private:
		BattleItem *_target;
		BattleAction::Action _action;
		Ability *_ability;
		int _value;
	};
}

#endif