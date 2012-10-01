#include "../ProgramDefaults.h"
#include "../Headers/BattleScene.h"

using namespace playmodule;

BattleScene::BattleScene(QObject *parent, SoundItem *sound)
	: QGraphicsScene(parent), _party(0), _group(0), _enemies(0), _activeEnemies(0),
	_characters(0), _activeCharacters(0), _experience(0), _currency(0),
	_sound(sound), _turn(new BattleAction()), _nextStep(0), _battleAI(0) {
		setBackgroundBrush(QBrush(Qt::black));
		setSceneRect(0, 0, 800, 600);

		_background = new BattleBackgroundItem(0);
		addItem(_background);
		_background->setPos(0, 0);
		_background->hide();

		_characterStatus = new BattleCharacterStatus(0);
		addItem(_characterStatus);
		_characterStatus->setPos(0, 400);
		_characterStatus->hide();

		_messageBoard = new BattleMessage(0);
		addItem(_messageBoard);
		_messageBoard->setPos(400, 400);
		_messageBoard->hide();

		// Pointer cannot be added to the scene before the background...
		_targetPointer = new BattleTargetPointer(0);
		addItem(_targetPointer);
		_targetPointer->hide();

		_abilitySelection = new BattleAbilitySelection(0);
		addItem(_abilitySelection);
		_abilitySelection->setPos(400, 400);
		_abilitySelection->hide();

		_targetSelection = new BattleTargetSelection(0, _targetPointer);
		addItem(_targetSelection);
		_targetSelection->setPos(400, 400);
		_targetSelection->hide();

		connect(_abilitySelection, SIGNAL(actionSelected(BattleAction::Action)), this, SLOT(actionSelected(BattleAction::Action)));
		connect(_targetSelection, SIGNAL(targetSelected(BattleItem*)), this, SLOT(targetSelected(BattleItem*)));
}

BattleScene::~BattleScene() {}

void BattleScene::initialize(const QPixmap &battleback, Battlegroup *group, Party *party) {
	// Setup background
	_background->setBackground(battleback);
	_background->show();

	// Setup characters and enemies
	setEnemies(group);
	setCharacters(party);
	_characterStatus->show();

	// Setup battle A.I.
	QVector<BattleItem*> AIOpponents(0);
	for (int i = 0; i < _activeCharacters.size(); i++)
		AIOpponents.append(_activeCharacters.at(i));

	QVector<BattleItem*> AIBattlers(0);
	for (int i = 0; i < _activeEnemies.size(); i++)
		AIBattlers.append(_activeEnemies.at(i));

	_battleAI = new BattleAI();
	_battleAI->initialize(AIOpponents, AIBattlers, _turn);

	// Setup sound
	if (group->getBackgroundMusic() != QString())
		_sound->setBackgroundSource(Phonon::MediaSource(g_backgroundMusicPath + group->getBackgroundMusic()));
	else
		_sound->stop();

	// Setup encounter message
	_messageBoard->setMessage(QString("Encountering ") + group->getName() + ".");
	_messageBoard->show();
	_targetPointer->hide();
	_targetSelection->hide();
	_abilitySelection->hide();
	_nextStep = &BattleScene::fillQueue;
}

void BattleScene::finalize() {
	// Reset all panels and hide them
	_messageBoard->setMessage(QString());
	_messageBoard->hide();
	_background->hide();
	_targetPointer->hide();
	_characterStatus->hide();
	_targetSelection->hide();
	_abilitySelection->hide();

	// Clear enemies
	for (int i = 0; i < _enemies.size(); i++) {
		BattleEnemyItem *it = _enemies.at(i);
		if (!!it) {
			it->setParentItem(0);
			removeItem(it);
			delete it;
		}
	}
	_enemies.clear();
	_activeEnemies.clear();

	// Clear characters
	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *it = _characters.at(i);
		if (!!it) {
			it->setParentItem(0);
			removeItem(it);
		}
	}
	_characters.clear();
	_activeCharacters.clear();
	_battlersQueue.clear();
	_party = 0;
	_group = 0;

	// Clear loot and experience
	_experience = 0;
	_currency = 0;
	_loot.clear();

	// Reset turn
	_nextStep = 0;
	_turn->setBattler(0);
	_turn->setTarget(0);
	_turn->setAction(BattleAction::ATTACK);
	_turn->setAbility(0);

	// Delete the AI
	delete _battleAI;
	_battleAI = 0;
}

void BattleScene::setCharacters(Party *party) {
	_party = party;
	QVector<BattleCharacterItem*> characters = party->getPartyMembers();
	int x = 50;
	for (int i = 0; i < g_maxBattlegroupsize; i++) {
		BattleCharacterItem *it = characters.at(i);
		if (!!it) {
			it->setHealth(it->getMaxHealth().second);
			it->setEnergy(it->getEnergy().second);

			it->setParentItem(_background);

			int y = _background->pixmap().height() - it->pixmap().height() - 25;
			it->setPos(x, y);
			x = x + it->pixmap().width() + 50;

			_characters.append(it);
			_activeCharacters.append(it);
		}
	}

	_characterStatus->setCharacters(_characters);
}

void BattleScene::setEnemies(Battlegroup *group) {
	_group = group;
	QVector<Enemy*> enemies = group->getEnemies();
	int x = 800;
	for (int i = 0; i < enemies.size(); i++) {
		Enemy *it = enemies.at(i);
		if (!!it) {
			BattleEnemyItem *enemyItem = new BattleEnemyItem(_background);
			enemyItem->initialize(it);

			x = x - enemyItem->pixmap().width() -50;
			enemyItem->setPos(x, 25);

			_enemies.append(enemyItem);
			_activeEnemies.append(enemyItem);
		}
	}
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
	if (_abilitySelection->isVisible()) {
		_abilitySelection->keyPressEvent(event);
	} else if (_targetSelection->isVisible()) {
		switch (event->key()) {
		case Qt::Key_Escape:
		case Qt::Key_Backspace:
			characterTurn();
			break;
		default:
			_targetSelection->keyPressEvent(event);
		}
	} else {
		switch (event->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
			event->accept();
			if (!!_nextStep)
				(this->*_nextStep)();
			else
				emit battleFinished(BattleScene::WIN);
		}
	}
}

void BattleScene::fillQueue() {
	_messageBoard->hide();
	QVector<BattleItem*> battlers(0);
	
	// Only select characters that are not KO
	for (int i = 0; i < _activeCharacters.size(); i++) {
		BattleItem *it = _activeCharacters.at(i);
		if (it->getHealth().second > 0)
			battlers.append(it);
	}

	// Only select enemies that are not KO
	for (int i = 0; i < _activeEnemies.size(); i++) {
		BattleItem *it = _activeEnemies.at(i);
		if (it->getHealth().second > 0)
			battlers.append(it);
	}
	
	// Fill the battler's queue with the highest agility battlers first
	_battlersQueue.clear();
	while (battlers.size() != 0) {
		int index = 0;
		int maxAgility = 0;
		for (int i = 0; i < battlers.size(); i++) {
			BattleItem *it = battlers.at(i);
			if (it->getAgility().second > maxAgility) {
				index = i;
				maxAgility = it->getAgility().second;
			}
		}

		_battlersQueue.append(battlers.at(index));
		battlers.remove(index);
	}

	startTurn();
}

void BattleScene::startTurn() {
	if (_battlersQueue.size() > 0) {
		_turn->setBattler(_battlersQueue.first());
		_battlersQueue.remove(0);

		_messageBoard->setMessage(_turn->getBattler()->getName() + QString(" can make the next move."));
		_messageBoard->show();
		_targetPointer->hide();
		_targetSelection->hide();
		_abilitySelection->hide();
	}
	
	if (_turn->getBattler()->getType() == BattleItem::CHARACTER)
		_nextStep = &BattleScene::characterTurn;
	else if (_turn->getBattler()->getType() == BattleItem::ENEMY)
		_nextStep = &BattleScene::enemyTurn;
}

void BattleScene::characterTurn() {
	_abilitySelection->setBattler(dynamic_cast<BattleCharacterItem*>(_turn->getBattler()));
	_messageBoard->hide();
	_targetPointer->hide();
	_targetSelection->hide();
	_abilitySelection->show();
}

void BattleScene::enemyTurn() {
	_battleAI->search();
	actionEvaluation();
}

void BattleScene::actionEvaluation() {
	switch (_turn->getAction()) {
	case BattleAction::ATTACK:
		executeAttack();
		break;
	case BattleAction::ABILITY:
		executeAbility();
		break;
	case BattleAction::FLEE:
		QString message("Cannot flee from this battle...");
		_messageBoard->setMessage(message);
		break;
	}

	_messageBoard->show();
	_targetPointer->hide();
	_targetSelection->hide();
	_abilitySelection->hide();

	_nextStep = &BattleScene::turnResults;
}

void BattleScene::turnResults() {
	_messageBoard->show();
	_targetPointer->hide();
	_targetSelection->hide();
	_abilitySelection->hide();

	// Check for a knock out
	if ((_turn->getAction() != BattleAction::FLEE) && (_turn->getTarget()->getHealth().second <= 0)) {
		_turn->getTarget()->hide();
		if (_battlersQueue.indexOf(_turn->getTarget()) != (-1))
			_battlersQueue.remove(_battlersQueue.indexOf(_turn->getTarget()));

		if (_turn->getTarget()->getType() == BattleItem::CHARACTER)
			_activeCharacters.remove(_activeCharacters.indexOf(dynamic_cast<BattleCharacterItem*>(_turn->getTarget())));
		else if (_turn->getTarget()->getType() == BattleItem::ENEMY)
			_activeEnemies.remove(_activeEnemies.indexOf(dynamic_cast<BattleEnemyItem*>(_turn->getTarget())));

		QString message(_turn->getTarget()->getName() + " is knocked out");
		_messageBoard->setMessage(message);

		// If a character is knocked out, notify the A.I.
		if (_turn->getTarget()->getType() == BattleItem::CHARACTER)
			_battleAI->opponentKO(_turn->getTarget());
		if (_turn->getTarget()->getType() == BattleItem::ENEMY)
			_battleAI->battlerKO(_turn->getTarget());

		// If one side is completely knocked out, finish the battle
		if (_activeEnemies.size() == 0) {
			endBattle();
		} else if (_activeCharacters.size() == 0) {
			finishBattleScene();
		} else {
			if (_battlersQueue.empty())
				_nextStep = &BattleScene::fillQueue;
			else
				_nextStep = &BattleScene::startTurn;
		}

	} else { // Else just continue
		if (_battlersQueue.empty())
			fillQueue();
		else
			startTurn();
	}
}

void BattleScene::endBattle() {
	if (_group->getVictoryMusic() != QString())
		_sound->setBackgroundSource(g_backgroundMusicPath + _group->getVictoryMusic());
	else
		_sound->stop();

	for (int i = 0; i < _enemies.size(); i++) {
		BattleEnemyItem *enemy = _enemies.at(i);
		QVector<QPair<EquipmentPiece*, int>> drops = enemy->getEnemy()->getDrops()->getItems();

		for (int j = 0; j < drops.size(); j++) {
			if ((qrand() % 101) <= drops.at(j).second) {
				EquipmentPiece *drop = drops.at(j).first;

				QVector<QPair<EquipmentPiece*, int>>::iterator it;
				for (it = _loot.begin(); it != _loot.end(); it++) {
					if (it->first == drop)
						it->second = it->second + 1;
				}

				if (it == _loot.end())
					_loot.append(QPair<EquipmentPiece*, int>(drop, 1));
			}
		}

		QPair<int, int> currencyDrop = enemy->getEnemy()->getDrops()->getCurrency();
		_currency += qrand() % ((currencyDrop.second + 1) - currencyDrop.first) + currencyDrop.first;
		_experience += enemy->getEnemy()->getDrops()->getExperience();
	}

	characterProgression();
}

void BattleScene::characterProgression() {
	QString message;
	message.append("Your characters earned " + QString::number(_experience) + " experience.\n");

	for (int i = 0; i < _activeCharacters.size(); i++) {
		BattleCharacterItem *charater = _activeCharacters.at(i);

		int oldLevel = charater->getLevel();
		charater->addExperience(_experience);
		int newLevel = charater->getLevel();
		if (oldLevel != newLevel)
			message.append(charater->getName() + " is now level " + QString::number(newLevel) + "!\n");
	}

	_messageBoard->setMessage(message);
	_nextStep = &BattleScene::distributeLoot;
}

void BattleScene::distributeLoot() {
	QString message;
	message.append("Your characters found:\n");
	message.append(QString::number(_currency) + " gold.\n");
	_party->getInventory()->addCurrency(_currency);

	for (int i = 0; i < _loot.size(); i++) {
		QPair<EquipmentPiece*, int> drop = _loot.at(i);
		_party->getInventory()->addEquipment(drop.first, drop.second);
		message.append(drop.first->getName() + " (x " + QString::number(drop.second) + ")\n");
	}

	_messageBoard->setMessage(message);
	_nextStep = 0;
}

void BattleScene::finishBattleScene() {
	if (_activeEnemies.empty())
		emit battleFinished(BattleScene::WIN);
	else if (_activeCharacters.empty())
		emit battleFinished(BattleScene::LOSE);
	else
		emit battleFinished(BattleScene::FLEE);
	_nextStep = 0;
}

void BattleScene::executeAttack() {
	BattleItem *battler = _turn->getBattler(), *target = _turn->getTarget();
	QPair<BattleItem::ResultType, int> hit = target->takeHit(battler, battler->getElements(), battler->getPrimaryAttribute());

	QString message;
	if (hit.first != BattleItem::MISS) {
		target->setHealth(target->getHealth().second - hit.second);
		message.append(battler->getName() + " launches an attack on " + target->getName() + " dealing " + QString::number(hit.second) + " damage. ");
		switch (hit.first) {
		case BattleItem::VULNERABLE:
			message.append(target->getName() + " took a serious hit!");
			break;
		case BattleItem::RESIST:
			message.append(target->getName() + " didn't even flinch...");
			break;
		}

		if ((_turn->getBattler()->getType() == BattleItem::ENEMY) && (_turn->getTarget()->getType() == BattleItem::CHARACTER))
			_battleAI->updateDefensiveInfo();

	} else {
		message.append(battler->getName() + " launches an attack on " + target->getName() + " but misses. ");
	}
	_messageBoard->setMessage(message);
}

void BattleScene::executeAbility() {
	if (_turn->getAbility()->getType() == Ability::OFFENSIVE)
		executeOffensiveAbility();
	else if (_turn->getAbility()->getType() == Ability::DEFENSIVE)
		executeDefensiveAbility();
}

void BattleScene::executeOffensiveAbility() {
	BattleItem *battler = _turn->getBattler(), *target = _turn->getTarget();
	Ability *ability = _turn->getAbility();
	Attribute *attribute = ability->getAttribute().first;
	int value = battler->getOffensive(attribute->getName()).second * ability->getAttribute().second;

	// Calculate the damage taken by the target
	QPair<BattleItem::ResultType, int> hit = target->takeHit(battler, ability->getElements(), QPair<Attribute*, int>(attribute, value));
	battler->setEnergy(battler->getEnergy().second - ability->getCost());

	// If it hit, then display damage message
	QString message;
	if (hit.first != BattleItem::MISS) {
		target->setHealth(target->getHealth().second - hit.second);
		message.append(battler->getName() + " uses " + ability->getName() + " on " + target->getName() + " dealing " + QString::number(hit.second) + " damage. ");
		switch (hit.first) {
		case BattleItem::VULNERABLE:
			message.append(target->getName() + " took a serious hit!");
			break;
		case BattleItem::RESIST:
			message.append(target->getName() + " didn't even flinch...");
			break;
		}

		if ((_turn->getBattler()->getType() == BattleItem::ENEMY) && (_turn->getTarget()->getType() == BattleItem::CHARACTER))
			_battleAI->updateDefensiveInfo();

		
	} else { // If the ability missed...
		message.append(battler->getName() + " uses " + ability->getName() + " on " + target->getName() + " but misses. ");
	}

	_messageBoard->setMessage(message);
}

void BattleScene::executeDefensiveAbility() {
	BattleItem *battler = _turn->getBattler(), *target = _turn->getTarget();
	Ability *ability = _turn->getAbility();
	Attribute *attribute = ability->getAttribute().first;
	int value = battler->getOffensive(attribute->getName()).second * ability->getAttribute().second;

	// Calculate the recovery
	QPair<BattleItem::ResultType, int> recovery = target->recover(ability->getElements(), QPair<Attribute*, int>(attribute, value));
	battler->setEnergy(battler->getEnergy().second - ability->getCost());

	target->setHealth(target->getHealth().second + recovery.second);
	QString message(battler->getName() + " uses " + ability->getName() + " on " + target->getName() + " and recovers " + QString::number(recovery.second) + " " + target->getHealth().first->getName() + ". ");
	switch (recovery.first) {
	case BattleItem::VULNERABLE:
		message.append(target->getName() + " feels relieved.");
	case BattleItem::RESIST:
		message.append(target->getName() + " still feels weak...");
	}

	_messageBoard->setMessage(message);
}

void BattleScene::actionSelected(BattleAction::Action action) {
	_turn->setAction(action);

	switch (action) {
	case BattleAction::ATTACK:
		_targetSelection->setBattlers(_activeCharacters, _activeEnemies);
		_targetSelection->setTarget(_activeEnemies.at(0));
		_messageBoard->hide();
		_targetPointer->show();
		_targetSelection->show();
		_abilitySelection->hide();
		break;
	case BattleAction::ABILITY:
		_turn->setAbility(_abilitySelection->getAbility());
		_targetSelection->setBattlers(_activeCharacters, _activeEnemies);

		if (_abilitySelection->getAbility()->getType() == Ability::OFFENSIVE)
			_targetSelection->setTarget(_activeEnemies.at(0));
		else if (_abilitySelection->getAbility()->getType() == Ability::DEFENSIVE)
			_targetSelection->setTarget(_activeCharacters.at(0));

		_messageBoard->hide();
		_targetPointer->show();
		_targetSelection->show();
		_abilitySelection->hide();
		break;
	case BattleAction::FLEE:
		actionEvaluation();
		break;
	}
}

void BattleScene::targetSelected(BattleItem *target) {
	_turn->setTarget(target);
	actionEvaluation();
}