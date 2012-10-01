#include "../Logger.h"
#include "../Headers/Party.h"
#include "../ProgramDefaults.h"

using namespace playmodule;

Party::Party()
	: _characters(QVector<BattleCharacterItem*>(g_maxPartysize, 0)), _inventory(new Inventory()) {}

Party::Party(QVector<Character*> characters)
	: _characters(QVector<BattleCharacterItem*>(g_maxPartysize, 0)), _inventory(new Inventory()) {

		for (int i = 0; i < characters.size(); i++) {
			if (!!characters.at(i)) {
				BattleCharacterItem *character = new BattleCharacterItem(0);
				character->initialize(characters.at(i));
					_characters.replace(i, character);

				QVector<EquipmentPiece*> equipment = characters.at(i)->getEquipment()->getEquipment();
				
				for (int i = 0; i < equipment.size(); i++) {
					if ((i < 2) && !!equipment.at(i))
						character->getEquipment()->setWeapon(dynamic_cast<Weapon*>(equipment.at(i)));
					else if ((i < 7) && !!equipment.at(i))
						character->getEquipment()->setArmor(dynamic_cast<Armor*>(equipment.at(i)));
					else if (!!equipment.at(i))
						character->getEquipment()->setAccessory(dynamic_cast<Accessory*>(equipment.at(i)));
				}

			}
		}
}

Party::~Party() {
	for (int i = 0; i < _characters.size(); i++) {
		if (!!_characters.at(i))
			delete _characters.at(i);
	}
	_characters.clear();

	delete _inventory;
}

Inventory* Party::getInventory() const {
	return _inventory;
}

BattleCharacterItem* Party::getPartyLeader() const {
	return _characters.at(0);
}

BattleCharacterItem* Party::getPartyMember(int position) const {
	if ((position < 0) || (position >= g_maxPartysize))
		throw PlayException("The position of the party member does not exist.");

	return _characters.at(position);
}

BattleCharacterItem* Party::getPartyMember(QString name) const {
	if (name == QString())
		throw PlayException("Cannot search for a character with an empty name.");

	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *it = _characters.at(i);
		if (!!it && (name == it->getName()))
			return it;
	}

	return 0;
}

QVector<BattleCharacterItem*> Party::getPartyMembers() const {
	QVector<BattleCharacterItem*> characters(0);
	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *it  = _characters.at(i);
		if (!!it)
			characters.append(it);
	}

	return characters;
}

void Party::setPartyLeader(BattleCharacterItem *character) {
	if (!character)
		throw PlayException("Cannot set a null party leader.");

	_characters.replace(0, character);
}

void Party::setPartyMember(int position, BattleCharacterItem *character) {
	if ((position < 0) || (position >= g_maxPartysize))
		throw PlayException("The position of the party member does not exist.");

	if (!character)
		throw PlayException("Cannot set a null party member.");

	if (!!getPartyMember(character->getName()))
		throw PlayException("The character is already a member of the party.");

	_characters.replace(position, character);
}

void Party::switchPlace(int position1, int position2) {
	if ((position1 < 0) || (position1 >= g_maxPartysize))
		throw PlayException("The position of the party member does not exist.");

	if ((position2 < 0) || (position2 >= g_maxPartysize))
		throw PlayException("The position of the party member does not exist.");

	BattleCharacterItem *char1 = _characters.at(position1);
	BattleCharacterItem *char2 = _characters.at(position2);
	_characters.replace(position1, char2);
	_characters.replace(position2, char1);
}

void Party::switchPlace(QString name1, QString name2) {
	if ((name1 == QString()) || (name2 == QString()))
		throw PlayException("Cannot search for a character with an empty name.");

	if (name1 == name2)
		throw PlayException("Cannot switch twp characters that have the same name.");

	if (!getPartyMember(name1))
		throw PlayException("The character with name" + name1 + " is no member of the party.");

	if (!getPartyMember(name2))
		throw PlayException("The character with name" + name2 + " is no member of the party.");

	int pos1;
	int pos2;
	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *it = _characters.at(i);
		if (!!it) {
			if (it->getName() == name1)
				pos1 = i;

			if (it->getName() == name2)
				pos2 = i;
		}
	}

	switchPlace(pos1, pos2);
}

void Party::unsetPartyLeader() {
	if (!_characters.at(0))
		throw PlayException("Cannot unset the party leader because no leader is set.");

	_characters.replace(0, 0);
}

void Party::unsetPartyMember(int position) {
	if (!_characters.at(position))
		throw PlayException("Cannot unset the party member because no member is set at that location.");

	_characters.replace(position, 0);
}

void Party::unsetPartyMember(QString name) {
	if (!getPartyMember(name))
		throw PlayException("Cannot unset the party member because no member with that name is set.");

	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *it = _characters.at(i);
		if (!!it && (it->getName() == name))
			unsetPartyMember(i);
	}
}