#include "../Headers/BattleAction.h"

using namespace playmodule;

BattleAction::BattleAction()
	: _battler(0), _target(0), _ability(0) {}

BattleAction::~BattleAction() {}

BattleItem* BattleAction::getBattler() const {
	return _battler;
}

BattleItem* BattleAction::getTarget() const {
	return _target;
}

BattleAction::Action BattleAction::getAction() const {
	return _action;
}

Ability* BattleAction::getAbility() const {
	return _ability;
}

void BattleAction::setBattler(BattleItem *battler) {
	_battler = battler;
}

void BattleAction::setTarget(BattleItem *target) {
	_target = target;
}

void BattleAction::setAction(BattleAction::Action action) {
	_action = action;
}

void BattleAction::setAbility(Ability *ability) {
	_ability = ability;
}