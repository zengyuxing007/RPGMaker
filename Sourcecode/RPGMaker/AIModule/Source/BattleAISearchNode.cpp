#include "../Headers/BattleAISearchNode.h"

using namespace aimodule;

/**
*	Standard constructor.
*/
BattleAISearchNode::BattleAISearchNode()
	: _target(0), _action(BattleAction::ATTACK), _ability(0), _value(0) {}

/**
*	Standard destructor.
*/
BattleAISearchNode::~BattleAISearchNode() {}

/**
*	Get the target associated with this search node.
*/
BattleItem* BattleAISearchNode::getTarget() const {
	return _target;
}

/**
*	Get the action associated with this search node.
*/
BattleAction::Action BattleAISearchNode::getAction() const {
	return _action;
}

/**
*	Get the ability associated with this search node.
*	This value will be null if the action is not set to use an ability.
*/
Ability* BattleAISearchNode::getAbility() const {
	return _ability;
}

/**
*	Get the expected value associated with this search node.
*/
int BattleAISearchNode::getExpectedValue() const {
	return _value;
}

/**
*	Set the current target of this search node.
*/
void BattleAISearchNode::setTarget(BattleItem *target) {
	_target = target;
}

/**
*	Set the current action of this search node.
*/
void BattleAISearchNode::setAction(BattleAction::Action action) {
	_action = action;
}

/**
*	Set the current ability of this search node.
*/
void BattleAISearchNode::setAbility(Ability *ability) {
	_ability = ability;
}

/**
*	set the current expected value of this search node.
*/
void BattleAISearchNode::setExpectedValue(int value) {
	_value = value;
}