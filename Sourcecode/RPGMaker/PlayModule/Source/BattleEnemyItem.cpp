#include "../ProgramDefaults.h"
#include "../Headers/BattleEnemyItem.h"

using namespace playmodule;

BattleEnemyItem::BattleEnemyItem(QGraphicsItem *parent)
	: BattleItem(parent, BattleItem::ENEMY) {}

BattleEnemyItem::~BattleEnemyItem() {}

QString BattleEnemyItem::getName() const {
	return _enemy->getName();
}

Enemy* BattleEnemyItem::getEnemy() const {
	return _enemy;
}

void BattleEnemyItem::initialize(Enemy *enemy) {
	_enemy = enemy;

	// Enemy settings
	_level = _enemy->getLevel();
	_experience = 0;
	_class = _enemy->getClass();
	_environmentImage = _enemy->getEnvironmentImage().first;
	_battleImage = _enemy->getBattleImage().first;

	setPixmap(_battleImage);

	// Elements
	_elements = _enemy->getElements();

	// Attributes
	QVector<QPair<Attribute*, QPair<int, double>>> attributes = _enemy->getClass()->getAttributes();
	for (int i = 0; i < attributes.size(); i++) {
		QPair<Attribute*, QPair<int, double>> attribute = attributes.at(i);
		switch (attribute.first->getType()) {
		case Attribute::LIFE:
			_health = QPair<Attribute*, int>(attribute.first, (attribute.second.first + (attribute.second.second * _enemy->getLevel())) * g_healthMultiplier);
			_attributes.append(_health);
			break;
		case  Attribute::ENERGY:
			_energy = QPair<Attribute*, int>(attribute.first, (attribute.second.first + (attribute.second.second * _enemy->getLevel())) * g_energyMultiplier);
			_attributes.append(_energy);
			break;
		default:
			_attributes.append(QPair<Attribute*, int>(attribute.first, (attribute.second.first + attribute.second.second * _enemy->getLevel())));
		}
	}

	// Abilities
	QVector<QPair<Ability*, int>> abilities = enemy->getAllAbilities();
	for (int i = 0; i < abilities.size(); i++) {
		QPair<Ability*, int> ability = abilities.at(i);
		if (ability.second <= _level)
			_abilities.append(ability.first);
	}
}