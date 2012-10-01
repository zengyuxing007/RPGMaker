#include "../Headers/BattleAI.h"
#include "../Headers/BattleAISearchNode.h"

using namespace aimodule;

/**
*	Standard constructor.
*/
BattleAI::BattleAI()
	: _turn(0), _opponents(0), _activeOpponents(0),
	_battlers(0), _activeBattlers(0) {}

/**
*	Standard destructor.
*	The destructor only destroys the entities it has created for itself
*	and keeps the provided characters and enemies intact.
*/
BattleAI::~BattleAI() {

	for (int i = 0; i < _opponents.size(); i++)
		delete _opponents.at(i).second;

	_opponents.clear();
	_activeOpponents.clear();

	_battlers.clear();
	_activeBattlers.clear();
}

/**
*	Initializes the Battle AI with the player's characters and their encounter.
*	The <i>turn</i> parameter is used to fill in the actions the Battle AI will take and lets the Battle AI know which character's turn it is.
*	This function only lets the Battle AI create some information needed to start searching for actions.
*	The actual search process is started with the function <i>search()</i>.
*/
void BattleAI::initialize(QVector<BattleItem*> opponents, QVector<BattleItem*> battlers, BattleAction *turn) {
	for (int i = 0; i < opponents.size(); i++) {
		QPair<BattleItem*, BattleAIBattlerInfo*> opponent(opponents.at(i), new BattleAIBattlerInfo(opponents.at(i)));

		// Initiate the AI with guess values;
		int level = opponent.first->getLevel();

		// Offensive attributes
		QVector<QPair<Attribute*, int>> offensiveAttributes = opponent.first->getOffensiveAttributes();
		for (int j = 0; j < offensiveAttributes.size(); j++) {
			QPair<Attribute*, int> it = offensiveAttributes.at(j);
			opponent.second->setOffensiveStatus(QPair<Attribute*, int>(it.first, level));
		}

		// Defensive attributes
		QVector<QPair<Attribute*, int>> defensiveAttributes = opponent.first->getDefensiveAttributes();
		for (int j = 0; j < defensiveAttributes.size(); j++) {
			QPair<Attribute*, int> it = defensiveAttributes.at(j);
			opponent.second->setDefensiveStatus(QPair<Attribute*, int>(it.first, level));
		}

		_opponents.append(opponent);
		_activeOpponents.append(opponent);
	}

	// Set the battlers that are under control of the A.I.
	for (int i = 0; i < battlers.size(); i++) {
		_battlers.append(battlers.at(i));
		_activeBattlers.append(battlers.at(i));
	}


	_turn = turn;
}

/**
*	This function should be called by the battle scene when a character of the player is knocked out.
*	This will remove the character from the list so that the Battle AI will no longer target that character.
*/
void BattleAI::opponentKO(BattleItem *opponent) {
	for (int i = 0; i < _activeOpponents.size(); i++) {
		if (_activeOpponents.at(i).first == opponent) {
			_activeOpponents.remove(i);
			return;
		}
	}
}

/**
*	This function should be called by the battle scene when a character under the control of the AI is knocked out.
*	This will remove the character from the list so that the Battle AI will no longer keep track of it.
*/
void BattleAI::battlerKO(BattleItem *battler) {
	for (int i = 0; i < _activeBattlers.size(); i++) {
		if (_activeBattlers.at(i) == battler) {
			_activeBattlers.remove(i);
			return;
		}
	}
}

/**
*	Starts the actual search process to find the best action that can be done by the current character.
*	The Battle AI will frst analyse the party's overall status. If it finds that it needs attention,
*	it will go in a defensive mode in which it will try to recover some health. If the party doesn't need
*	attention, it will go in offensive mode and will try to hit the player as hard as possible.
*
*	If the Battle AI is in offensive mode, it will search for the best regular attack it can perform
*	and the best ability it can use against its opponents. Although an ability will almost always outperform
*	a regular attack, there is still a great chance the Battle AI will still use the regular attack. This is because
*	using all available energy might render this character useless if the AI decides to go in defensive mode.
*
*	If the Battle AI is in defensive mode, it will try to search for the most cost effective solution, if any.
*	The most cost effective solution is based on how much health is healed per point of energy that is invested
*	with the selected ability. This value is also influenced by how much influence the amount of health has in comparison
*	to the total health of the target. If no suitable defensive solution can be found, the Battle AI will switch to offensive mode instead.
*/
void BattleAI::search() {
	// Calculate party status
	bool defensiveTrigger = false;
	int initialStatus = 0, currentStatus = 0;
	for (int i = 0; i < _activeBattlers.size(); i++) {
		BattleItem *battler = _activeBattlers.at(i);
		initialStatus += battler->getMaxHealth().second;
		currentStatus += battler->getHealth().second;

		int currentHealth = battler->getHealth().second, initialHealth = battler->getMaxHealth().second;

		// If one character is in critical condition, then set the trigger
		double statusCheck = ((double)currentHealth / (double)initialHealth) * 100;
		if (statusCheck < 50)
			defensiveTrigger = true;
	}

	// If the party is in bad condition, then go in defensive mode
	currentStatus = currentStatus / initialStatus * 100;
	initialStatus = 100;
	if (defensiveTrigger || (currentStatus < initialStatus / _activeBattlers.size()))
		searchDefensiveAction();
	else
		searchOffensiveAction();
}

/**
*	Function process not filled in...
*/
void BattleAI::updateOffensiveInfo() {

}

/**
*	Let the Battle AI generate feedback from its action. This will update the defensive info
*	the Battle AI keeps track of of the character that has been targeted. This defensive info
*	influences the choice of a target when in offensive mode.
*/
void BattleAI::updateDefensiveInfo() {
	if ((_turn->getAction() == BattleAction::ATTACK) || (_turn->getAction() == BattleAction::ABILITY)) {

		// Search for the target's info node
		BattleItem *target = _turn->getTarget();
		BattleAIBattlerInfo *infoNode = 0;
		for (int i = 0; i < _activeOpponents.size(); i++) {
			if (_activeOpponents.at(i).first == target)
				infoNode = _activeOpponents.at(i).second;
		}

		// Update the defensive attribute of the target
		Attribute *offensiveAttribute = 0;
		if (_turn->getAction() == BattleAction::ATTACK)
			offensiveAttribute = _turn->getBattler()->getPrimaryAttribute().first;
		else
			offensiveAttribute = _turn->getAbility()->getAttribute().first;

		QPair<Attribute*, int> defensiveAttribute = target->getDefensive(offensiveAttribute->getLink()->getName());
		infoNode->setDefensiveStatus(defensiveAttribute);

		// Update the element stats of the target
		QVector<Element*> targetElements = target->getElements();
		QVector<Element*> battlerElements(0);
		if (_turn->getAction() == BattleAction::ATTACK)
			battlerElements = _turn->getBattler()->getElements();
		else
			battlerElements = _turn->getAbility()->getElements();

		for (int i = 0; i < targetElements.size(); i++) {

			Element *targetElement = targetElements.at(i);
			for (int j = 0; j < battlerElements.size(); j++) {

				Element *battlerElement = battlerElements.at(j);
				if (targetElement->isResistant(battlerElement))
					infoNode->setElementStatus(QPair<Element*, BattleAIBattlerInfo::ElementStatus>(battlerElement, BattleAIBattlerInfo::RESIST));
				else if (targetElement->isVulnerable(battlerElement))
					infoNode->setElementStatus(QPair<Element*, BattleAIBattlerInfo::ElementStatus>(battlerElement, BattleAIBattlerInfo::VULNERABLE));
				else
					infoNode->setElementStatus(QPair<Element*, BattleAIBattlerInfo::ElementStatus>(battlerElement, BattleAIBattlerInfo::NONE));
			}
		}

	}
}

/**
*	If the Battle AI is in offensive mode, it will search for the best regular attack it can perform
*	and the best ability it can use against its opponents. Although an ability will almost always outperform
*	a regular attack, there is still a great chance the Battle AI will still use the regular attack. This is because
*	using all available energy might render this character useless if the AI decides to go in defensive mode.
*/
void BattleAI::searchOffensiveAction() {
	BattleItem *battler = _turn->getBattler();

	// Go over all offensive possibilities
	QVector<BattleAISearchNode*> offensiveAttacks(0);
	QVector<BattleAISearchNode*> offensiveAbilities(0);
	for (int i = 0; i < _activeOpponents.size(); i++) {
		QPair<BattleItem*, BattleAIBattlerInfo*> characterInfo = _activeOpponents.at(i);

		// First, evaluate a regular attack
		{
			BattleAISearchNode *searchNode = new BattleAISearchNode();
			searchNode->setTarget(characterInfo.first);
			searchNode->setAction(BattleAction::ATTACK);
			searchNode->setAbility(0);
			searchNode->setExpectedValue(attackExpectation(characterInfo));
			offensiveAttacks.append(searchNode);
		}

		// Second, evaluate all abilities for which enough energy is available
		QVector<Ability*> abilities = battler->getAbilities();
		for (int j = 0; j < abilities.size(); j++) {
			Ability *ability = abilities.at(j);
			if ((ability->getType() == Ability::OFFENSIVE) && (battler->getEnergy().second >= ability->getCost())) {

				BattleAISearchNode *searchNode = new BattleAISearchNode();
				searchNode->setTarget(characterInfo.first);
				searchNode->setAction(BattleAction::ABILITY);
				searchNode->setAbility(ability);
				searchNode->setExpectedValue(offensiveAbilityExpectation(characterInfo, ability));
				offensiveAbilities.append(searchNode);

			}
		}
	}

	// Select best attack move
	BattleAISearchNode *bestAttack = 0;
	for (int i = 0; i < offensiveAttacks.size(); i++) {
		BattleAISearchNode *attackNode = offensiveAttacks.at(i);
		if (!!bestAttack) {
			if (bestAttack->getExpectedValue() < attackNode->getExpectedValue())
				bestAttack = attackNode;
			else if (bestAttack->getExpectedValue() == attackNode->getExpectedValue()) {
				int randomNr = qrand() % 101;
				if (randomNr < 50)
					bestAttack = attackNode;
			}

		} else
			bestAttack = attackNode;
	}

	// Select best ability move if there are abilities available
	BattleAISearchNode *bestAbility = 0;
	if (offensiveAbilities.size() > 0) {
		for (int i = 0; i < offensiveAbilities.size(); i++) {
			BattleAISearchNode *abilityNode = offensiveAbilities.at(i);
			if (!!bestAbility) {
				if (bestAbility->getExpectedValue() < abilityNode->getExpectedValue())
					bestAbility = abilityNode;
				else if (bestAbility->getExpectedValue() == abilityNode->getExpectedValue()) {
					int randomNr = qrand() % 101;
					if (randomNr < 50)
						bestAbility = abilityNode;
				}

			} else
				bestAbility = abilityNode;
		}
	}

	// Select the action
	BattleAISearchNode *action = 0;
	if (!!bestAttack && !!bestAbility) {
		int randomNr = qrand() % 101;
		
		if (randomNr < 75)
			action = bestAbility;
		else
			action = bestAttack;

	} else {
		action = bestAttack;
	}

	_turn->setTarget(action->getTarget());
	_turn->setAction(action->getAction());
	_turn->setAbility(action->getAbility());
}

/**
*	If the Battle AI is in defensive mode, it will try to search for the most cost effective solution, if any.
*	The most cost effective solution is based on how much health is healed per point of energy that is invested
*	with the selected ability. This value is also influenced by how much influence the amount of health has in comparison
*	to the total health of the target. If no suitable defensive solution can be found, the Battle AI will switch to offensive mode instead.
*/
void BattleAI::searchDefensiveAction() {
	BattleItem *battler = _turn->getBattler();

	// Go over all defensive possibilities
	QVector<BattleAISearchNode*> defensiveAbilities(0);
	for (int i = 0; i < _activeBattlers.size(); i++) {
		BattleItem *target = _activeBattlers.at(i);

		// Evaluate all defensive possibilities for the target
		QVector<Ability*> abilities = battler->getAbilities();
		for (int j = 0; j < abilities.size(); j++) {
			Ability *ability = abilities.at(j);
			if ((ability->getType() == Ability::DEFENSIVE) && (battler->getEnergy().second >= ability->getCost())) {

				BattleAISearchNode *searchNode = new BattleAISearchNode();
				searchNode->setTarget(target);
				searchNode->setAction(BattleAction::ABILITY);
				searchNode->setAbility(ability);
				searchNode->setExpectedValue(defensiveAbilityExpectation(target, ability));

				defensiveAbilities.append(searchNode);
			}
		}
	}

	// Select the best ability if there are abilities available
	if (defensiveAbilities.size() > 0) {

		BattleAISearchNode *bestAbility = 0;
		for (int i = 0; i < defensiveAbilities.size(); i++) {
			BattleAISearchNode *abilityNode = defensiveAbilities.at(i);
			if (!!bestAbility) {

				if (bestAbility->getExpectedValue() < abilityNode->getExpectedValue())
					bestAbility = abilityNode;
				else if (bestAbility->getExpectedValue() == abilityNode->getExpectedValue()) {
					int randomNr = qrand() % 101;
					if (randomNr < 50)
						bestAbility = abilityNode;
				}

			} else
				bestAbility = abilityNode;
		}

		_turn->setTarget(bestAbility->getTarget());
		_turn->setAction(bestAbility->getAction());
		_turn->setAbility(bestAbility->getAbility());

	} else // Else search for an offensive action
		searchOffensiveAction();
}

/**
*	Calculates the expected effect of a regular attack on the selected target.
*/
int BattleAI::attackExpectation(QPair<BattleItem*, BattleAIBattlerInfo*> target) {
	return offensiveActionExpectation(target, _turn->getBattler()->getElements(), _turn->getBattler()->getPrimaryAttribute());
}

/**
*	Calculates the expected effect of the selected aility on a selected target.
*/
int BattleAI::offensiveAbilityExpectation(QPair<BattleItem*, BattleAIBattlerInfo*> target, Ability *ability) {
	int attributeValue = _turn->getBattler()->getOffensive(ability->getAttribute().first->getName()).second;
	attributeValue *= ability->getAttribute().second;

	QPair<Attribute*, int> attribute(0, 0);
	attribute.first = ability->getAttribute().first;
	attribute.second = attributeValue;
	return offensiveActionExpectation(target, ability->getElements(), attribute);
}

/**
*	Calculates the expected effect of an action on the selected target.
*	The expected effect takes the offensive attribute and weights it against the target's estimated corresponding defensive attribute.
*	The expected value is influenced by the elements of the current character and the elements of the target.
*/
int BattleAI::offensiveActionExpectation(QPair<BattleItem*, BattleAIBattlerInfo*> target, QVector<Element*> elements, QPair<Attribute*, int> attribute) {
	// Predict elemental influence
	// If the target is vulnerable => double the effect
	// If the target is resistant => half the effect
	int multiplier = 1;
	for (int i = 0; i < elements.size(); i++) {
		QPair<Element*, BattleAIBattlerInfo::ElementStatus> it = target.second->getElementStatus(elements.at(i)->getName());
		if (!!it.first) {
			switch (it.second) {
			case BattleAIBattlerInfo::VULNERABLE:
				multiplier *= 2;
				break;
			case BattleAIBattlerInfo::RESIST:
				multiplier /= 2;
				break;
			}
		}
	}

	// Predict expected damage
	QPair<Attribute*, int> defensiveAttribute = target.second->getDefensiveStatus(attribute.first->getLink()->getName());

	double expectedDamage = attribute.second * multiplier;
	double reductionPercentage = defensiveAttribute.second / (expectedDamage *2);

	return qMax<double>(0, expectedDamage - (expectedDamage * reductionPercentage));
}

/**
*	Calculates the expected effect the selected ability on the selected target.
*/
int BattleAI::defensiveAbilityExpectation(BattleItem *target, Ability *ability) {
	int attributeValue = _turn->getBattler()->getOffensive(ability->getAttribute().first->getName()).second;
	attributeValue *= ability->getAttribute().second;

	QPair<Attribute*, int> attribute(0, 0);
	attribute.first = ability->getAttribute().first;
	attribute.second = attributeValue;
	int expectedValue =  defensiveActionExcpectation(target, ability->getElements(), attribute);
	expectedValue /= ability->getCost();
	return expectedValue;
}

/**
*	Calculates the expected effect of a defensive action on the selected target.
*	An expected value of a defensive actions does not take into account the corresponding defensive attribute of the target
*	because it is assumed a character will not resist recovery. Elements still influence the expected value.
*/
int BattleAI::defensiveActionExcpectation(BattleItem *target, QVector<Element*> elements, QPair<Attribute*, int> attribute) {
	// Predict elemental influence
	// If the target is vulnerable => half the effect
	// If the target is resistant => double the effect
	int multiplier = 1;
	QVector<Element*> targetElements = target->getElements();
	for (int i = 0; i < elements.size(); i++) {
		Element *element = elements.at(i);

		for (int j = 0; j < targetElements.size(); j++) {
			Element *targetElement = targetElements.at(i);

			if (targetElement->isResistant(element))
				multiplier *= 2;
			else if (targetElement->isVulnerable(element))
				multiplier /= 2;
		}
	}

	// Predict expected effect
	// The effect is only limited to the maximum amount of health a target can recover!
	int expectedValue = attribute.second * multiplier;
	expectedValue = qMin<int>((target->getMaxHealth().second - target->getHealth().second), expectedValue);
	expectedValue = (double)expectedValue / (double)target->getMaxHealth().second * 100;
	return expectedValue;
}