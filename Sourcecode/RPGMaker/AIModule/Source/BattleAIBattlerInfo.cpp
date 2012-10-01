#include "../Headers/BattleAIBattlerInfo.h"
#include "../Components/Headers/Enemy.h"
#include "../Components/Headers/Character.h"

using namespace aimodule;

/**
*	Constructor. Initializes the info with estimations based on the level.
*/
BattleAIBattlerInfo::BattleAIBattlerInfo(BattleItem *battler)
	: _battler(battler), _offensiveAttributes(0), _defensiveAttributes(0), _elements(0) {}

/**
*	Standard destructor
*/
BattleAIBattlerInfo::~BattleAIBattlerInfo() {}

/**
*	Searches for an offensive attribute with <i>name</i> and returns the attribute
*	with the current associated value. If no such attribute is found, a null-pair is returned.
*/
QPair<Attribute*, int> BattleAIBattlerInfo::getOffensiveStatus(QString name) const {
	for (int i = 0; i < _offensiveAttributes.size(); i++) {
		QPair<Attribute*, int> it = _offensiveAttributes.at(i);
		if (it.first->getName() == name)
			return it;
	}

	return QPair<Attribute*, int>(0, 0);
}

/**
*	Searches for a defensive attribute with <i>name</i> and returns the attribute
*	with the current associated value. If no such attribute is found, a null-pair is returned.
*/
QPair<Attribute*, int> BattleAIBattlerInfo::getDefensiveStatus(QString name) const {
	for (int i = 0; i < _defensiveAttributes.size(); i++) {
		QPair<Attribute*, int> it = _defensiveAttributes.at(i);
		if (it.first->getName() == name)
			return it;
	}

	return QPair<Attribute*, int>(0, 0);
}

/**
*	Searches for an element with <i>name</i> and returns the element along with the status towards this element.
*	If no such element can be found, then a null-pair is returned.
*/
QPair<Element*, BattleAIBattlerInfo::ElementStatus> BattleAIBattlerInfo::getElementStatus(QString name) const {
	for (int i = 0; i < _elements.size(); i++) {
		QPair<Element*, BattleAIBattlerInfo::ElementStatus> it = _elements.at(i);
		if (it.first->getName() == name)
			return it;
	}

	return QPair<Element*, BattleAIBattlerInfo::ElementStatus>(0, BattleAIBattlerInfo::NONE);
}

/**
*	Updates the current knowlegde of the offensive attribute <i>attribute</i> of this character.
*/
void BattleAIBattlerInfo::setOffensiveStatus(QPair<Attribute*, int> attribute) {
	for (int i = 0; i < _offensiveAttributes.size(); i++) {
		QPair<Attribute*, int> it = _offensiveAttributes.at(i);
		if (it.first->getName() == attribute.first->getName()) {
			_offensiveAttributes.replace(i, attribute);
			return;
		}
	}

	_offensiveAttributes.append(attribute);
}

/**
*	Updates the current knowledge of the defensive attribute <i>attribute</i> of this character.
*/
void BattleAIBattlerInfo::setDefensiveStatus(QPair<Attribute*, int> attribute) {
	for (int i = 0; i < _defensiveAttributes.size(); i++) {
		QPair<Attribute*, int> it = _defensiveAttributes.at(i);
		if (it.first->getName() == attribute.first->getName()) {
			_defensiveAttributes.replace(i, attribute);
			return;
		}
	}

	_defensiveAttributes.append(attribute);
}

/**
*	Updates the current knowledge of the element <i>element</i> of this character.
*/
void BattleAIBattlerInfo::setElementStatus(QPair<Element*, ElementStatus> element) {
	for (int i = 0; i < _elements.size(); i++) {
		QPair<Element*, ElementStatus> it = _elements.at(i);
		if (it.first->getName() == element.first->getName()) {
			_elements.replace(i, element);
			return;
		}
	}

	_elements.append(element);
}