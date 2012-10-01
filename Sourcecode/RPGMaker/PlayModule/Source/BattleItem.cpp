#include "../Headers/BattleItem.h"
#include "../Components/Headers/ProjectTemplateFunctions.h"

using namespace playmodule;

BattleItem::BattleItem(QGraphicsItem *parent, BattleItem::Type type)
	: QGraphicsPixmapItem(parent), _type(type), _level(0), _experience(0),
	_class(0), _elements(0), _health(0, 0), _energy(0, 0), _attributes(0) {}

BattleItem::~BattleItem() {}

int BattleItem::getLevel() const {
	return _level;
}

int BattleItem::getExperience() const {
	return _experience;
}

Class* BattleItem::getClass() const {
	return _class;
}

QVector<Element*> BattleItem::getElements() const {
	return _elements;
}

QVector<Ability*> BattleItem::getAbilities() const {
	return _abilities;
}

QPair<Attribute*, int> BattleItem::getMaxHealth() const {
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getType() == Attribute::LIFE)
			return it;
	}

	return QPair<Attribute*, int>();
}

QPair<Attribute*, int> BattleItem::getMaxEnergy() const {
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getType() == Attribute::ENERGY)
			return it;
	}

	return QPair<Attribute*, int>();
}

QPair<Attribute*, int> BattleItem::getHealth() const {
	return _health;
}

QPair<Attribute*, int> BattleItem::getEnergy() const {
	return _energy;
}

QPair<Attribute*, int> BattleItem::getAgility() const {
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getType() == Attribute::AGILITY)
			return it;
	}

	return QPair<Attribute*, int>();
}

QPair<Attribute*, int> BattleItem::getEvasion() const {
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getType() == Attribute::EVASION)
			return it;
	}

	return QPair<Attribute*, int>();
}

QPair<Attribute*, int> BattleItem::getOffensive(QString name) const {
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getName() == name) {
			return it;
		}
	}
	return QPair<Attribute*, int>(0, 0);
}

QPair<Attribute*, int> BattleItem::getDefensive(QString name) const {
	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getName() == name) {
			return it;
		}
	}
	return QPair<Attribute*, int>(0, 0);
}

QPair<Attribute*, int> BattleItem::getPrimaryAttribute() const {
	return getOffensive(_class->getPrimaryAttribute()->getName());
}

QVector<QPair<Attribute*, int>> BattleItem::getOffensiveAttributes() const {
	QVector<QPair<Attribute*, int>> attributes(0);

	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getType() == Attribute::OFFENSIVE)
			attributes.append(it);
	}

	return attributes;
}

QVector<QPair<Attribute*, int>> BattleItem::getDefensiveAttributes() const {
	QVector<QPair<Attribute*, int>> attributes(0);

	for (int i = 0; i < _attributes.size(); i++) {
		QPair<Attribute*, int> it = _attributes.at(i);
		if (it.first->getType() == Attribute::DEFENSIVE)
			attributes.append(it);
	}

	return attributes;
}

BattleItem::Type BattleItem::getType() const {
	return _type;
}

void BattleItem::setHealth(int health) {
	int temp = qMax<int>(0, qMin<int>(health, getMaxHealth().second));
	_health.second = temp;
	emit valueChanged(getName());
}

void BattleItem::setEnergy(int energy) {
	int temp = qMax<int>(0, qMin<int>(energy, getMaxEnergy().second));
	_energy.second = temp;
	emit valueChanged(getName());
}

QPair<BattleItem::ResultType, int> BattleItem::takeHit(BattleItem *attacker, QVector<Element*> elements, QPair<Attribute*, int> offensiveAttribute) {
	int evasion = getEvasion().second;
	int agility = attacker->getAgility().second;
	int dodgeChance = qrand() % 101;

	// Calculate dodge chance
	if ((evasion >= agility) && (dodgeChance < (qAbs(evasion-agility)+12) ))
		return QPair<BattleItem::ResultType, int>(BattleItem::MISS, 0);
	else if ((evasion < agility) && (dodgeChance < qMax(evasion-agility+12, 0) ))
		return QPair<BattleItem::ResultType, int>(BattleItem::MISS, 0);

	QPair<BattleItem::ResultType, int> result(BattleItem::NONE, 0);

	// Calculate element multiplier
	double multiplier = 1;
	for (int i = 0; i < elements.size(); i++) {
		Element *battlerElement = elements.at(i);

		for (int j = 0; j < _elements.size(); j++) {
			Element *targetElement = _elements.at(i);

			if (targetElement->isResistant(battlerElement))
				multiplier /= 2;
			else if (targetElement->isVulnerable(battlerElement))
				multiplier *= 2;
		}
	}

	if (multiplier == 1)
		result.first = BattleItem::NONE;
	else if (multiplier > 1)
		result.first = BattleItem::VULNERABLE;
	else if (multiplier < 1)
		result.first = BattleItem::RESIST;

	// Calculate damage
	double damage = offensiveAttribute.second * multiplier;

	// Offense == Defense => 50% reduction // Defense >= 2*Offense => 100% reduction
	QPair<Attribute*, int> defensiveAttribute = getDefensive(offensiveAttribute.first->getLink()->getName());
	double reductionPercentage = defensiveAttribute.second / (damage * 2);
	
	result.second = qMax<double>(0, damage - (damage * reductionPercentage));

	return result;
}

QPair<BattleItem::ResultType, int> BattleItem::recover(QVector<Element*> elements, QPair<Attribute*, int> offensiveAttribute) {
	QPair<BattleItem::ResultType, int> result(BattleItem::NONE, 0);

	// Calculate element multiplier
	double multiplier = 1;
	for (int i = 0; i < elements.size(); i++) {
		Element *element = elements.at(i);

		for (int j = 0; j < _elements.size(); j++) {
			Element *targetElement = _elements.at(i);

			if (targetElement->isResistant(element))
				multiplier *= 2;
			else if (targetElement->isVulnerable(element))
				multiplier /= 2;
		}
	}

	if (multiplier == 1)
		result.first = BattleItem::NONE;
	else if (multiplier > 1)
		result.first = BattleItem::VULNERABLE;
	else if (multiplier < 1)
		result.first = BattleItem::RESIST;

	// Calculate recovery
	double recovery = offensiveAttribute.second * multiplier;
	recovery = qMin<int>((getMaxHealth().second - getHealth().second), recovery);
	result.second = recovery;

	return result;
}