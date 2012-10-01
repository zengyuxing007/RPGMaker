#include "../ProgramDefaults.h" 
#include "../Headers/BattleCharacterItem.h"

using namespace playmodule;

BattleCharacterItem::BattleCharacterItem(QGraphicsItem *parent)
	: BattleItem(parent, BattleItem::CHARACTER), _equipment(new BattleCharacterEquipment()) {}

BattleCharacterItem::~BattleCharacterItem() {
	delete _equipment;
}

QString BattleCharacterItem::getName() const {
	return _character->getName();
}

Character* BattleCharacterItem::getCharacter() const {
	return _character;
}

int BattleCharacterItem::getRequiredExperience() const {
	int required = 100;
	for (int i = 1; i < (_level+1); i++)
		required += (required * g_characterProgressionRate);

	return required;
}

BattleCharacterEquipment* BattleCharacterItem::getEquipment() const {
	return _equipment;
}

void BattleCharacterItem::initialize(Character *character) {
	_character = character;

	// Character settings
	_level = _character->getLevel();
	_experience = 0;
	_class = _character->getClass();
	_environmentImage = _character->getEnvironmentImage().first;

	if (_character->getBattleImage().first.width() > 200)
		_battleImage = _character->getBattleImage().first.scaled(200, 200, Qt::KeepAspectRatio);
	else
		_battleImage = _character->getBattleImage().first;

	setPixmap(_battleImage);

	// Elements
	_elements = _character->getElements();

	recalibrate();
}

void BattleCharacterItem::addExperience(int experience) {

	int required = getRequiredExperience();

	if ((_experience + experience) >= required) {
		_level++;
		experience -= (required - _experience);
		_experience = 0;
		recalibrate();
		addExperience(experience);
	} else 
		_experience += experience;
}

void BattleCharacterItem::recalibrate() {
	// Clear abilities and attributes
	_abilities.clear();
	_attributes.clear();
	
	// Attributes
	QVector<QPair<Attribute*, QPair<int, double>>> attributes = _character->getClass()->getAttributes();
	for (int i = 0; i < attributes.size(); i++) {
		QPair<Attribute*, QPair<int, double>> it = attributes.at(i);

		// Take level into account
		QPair<Attribute*, int> attribute(it.first, it.second.first + (it.second.second * _level));

		// Take equipment into account
		QVector<EquipmentPiece*> equipment = _equipment->getEquipment();
		for (int j = 0; j < equipment.size(); j++) {
			EquipmentPiece *it = equipment.at(j);
			if (!!it && !!it->getAttribute(attribute.first->getName()).first)
				attribute.second += it->getAttribute(attribute.first->getName()).second;
		}

		// Add attribute
		switch (attribute.first->getType()) {
		case Attribute::LIFE:
			_health = QPair<Attribute*, int>(attribute.first, attribute.second * g_healthMultiplier);
			_attributes.append(_health);
			break;
		case  Attribute::ENERGY:
			_energy = QPair<Attribute*, int>(attribute.first, attribute.second * g_energyMultiplier);
			_attributes.append(_energy);
			break;
		default:
			_attributes.append(attribute);
		}
	}

	// Abilities
	QVector<QPair<Ability*, int>> abilities = _character->getAllAbilities();
	for (int i = 0; i < abilities.size(); i++) {
		QPair<Ability*, int> ability = abilities.at(i);
		if (ability.second <= _level)
			_abilities.append(ability.first);
	}
}