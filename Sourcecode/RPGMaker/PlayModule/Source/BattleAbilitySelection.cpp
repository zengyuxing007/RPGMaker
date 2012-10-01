#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/BattleAbilitySelection.h"

using namespace playmodule;

BattleAbilitySelection::BattleAbilitySelection(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _currentBattler(0), _abilities(0),	_abilityIndex(0),
	_messageItem(0), _items(0), _activeItems(0), _state(ACTIONSELECTION), _action(BattleAction::ATTACK), _currentItem(0) {

		createWindow();
		QFont font ("Times", 12, QFont::Bold);

		_messageItem = new QGraphicsTextItem(this);
		_messageItem->setTextWidth(boundingRect().width() - 50);
		_messageItem->setDefaultTextColor(Qt::white);
		_messageItem->setFont(font);
		_messageItem->setPos(25, 10);

		int x = 25, y = 40;
		for (int i = 0; i < 4; i++) {
			QGraphicsTextItem *item = new QGraphicsTextItem(this);
			item->hide();
			item->setTextWidth(boundingRect().width() - 25);
			item->setDefaultTextColor(Qt::white);
			item->setFont(font);
			item->setPos(x, y);
			y += 30;

			_items.append(item);
		}
}

BattleAbilitySelection::~BattleAbilitySelection() {}

BattleAction::Action BattleAbilitySelection::getAction() const {
	return _action;
}

Ability* BattleAbilitySelection::getAbility() const {
	return _abilities.at(_abilityIndex);
}

void BattleAbilitySelection::setBattler(BattleItem *battler) {
	_currentBattler = battler;
	_abilities.clear();

	QVector<Ability*> abilities = battler->getAbilities();
	for (int i = 0; i < abilities.size(); i++) {
		Ability *ability = abilities.at(i);
		if (battler->getEnergy().second >= ability->getCost())
			_abilities.append(ability);
	}

	createActions();
}

void BattleAbilitySelection::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
		itemSelected();
		break;
	case Qt::Key_Backspace:
	case Qt::Key_Escape:
		createActions();
		break;
	case Qt::Key_Up:
		previousItem();
		break;
	case Qt::Key_Down:
		nextItem();
		break;
	}
}

void BattleAbilitySelection::createWindow() {
	// Get and resize the textballoon
	QPixmap windowSkin(QPixmap(g_windowskinPath + "001-Blue01.png"));
	QPixmap textBalloon(windowSkin.copy(0, 0, 128, 128));
	textBalloon = textBalloon.scaled(QSize(400, 200));

	// Paint the textballoon's border
	QPainter painter(&textBalloon);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	QPixmap borderTile(16, 16);

	// Northern border
	borderTile = windowSkin.copy(152, 0, 16, 16);
	for (int i = 1; i < textBalloon.width()/16; i++)
		painter.drawPixmap(borderTile.width() * i, 0, borderTile);

	// Southern border
	borderTile = windowSkin.copy(152, 48, 16, 16);
	for (int i = 1; i < textBalloon.width()/16; i++)
		painter.drawPixmap(borderTile.width() * i, textBalloon.height() - borderTile.height(), borderTile);

	// Western border
	borderTile = windowSkin.copy(128, 24, 16, 16);
	for (int i = 1; i < textBalloon.height()/16; i++)
		painter.drawPixmap(0, borderTile.height() * i, borderTile);

	// Eastern border
	borderTile = windowSkin.copy(176, 24, 16, 16);
	for (int i = 1; i < textBalloon.height()/16; i++)
		painter.drawPixmap(textBalloon.width() - borderTile.width(), borderTile.height() * i, borderTile);

	// North-West corner
	borderTile = windowSkin.copy(128, 0, 16, 16);
	painter.drawPixmap(0, 0, borderTile);

	// North-East corner
	borderTile = windowSkin.copy(176, 0, 16, 16);
	painter.drawPixmap(textBalloon.width()-16, 0, borderTile);

	// South-West corner
	borderTile = windowSkin.copy(128, 48, 16, 16);
	painter.drawPixmap(0, textBalloon.height()-16, borderTile);

	// South-East corner
	borderTile = windowSkin.copy(176, 48, 16, 16);
	painter.drawPixmap(textBalloon.width()-16, textBalloon.height()-16, borderTile);

	setPixmap(textBalloon);
}

void BattleAbilitySelection::createActions() {
	_state = BattleAbilitySelection::ACTIONSELECTION;
	_action = BattleAction::ATTACK;
	_abilityIndex = 0;

	_messageItem->setPlainText("Select an action for " + _currentBattler->getName() + ":");

	for (int i = 0; i < _items.size(); i++)
		_items.at(i)->hide();

	_activeItems.clear();

	_items.at(0)->setPlainText("Attack");
	_items.at(1)->setPlainText("Ability");
	_items.at(2)->setPlainText("Flee");

	_items.at(0)->show();
	_items.at(1)->show();
	_items.at(2)->show();

	_activeItems.append(_items.at(0));
	_activeItems.append(_items.at(1));
	_activeItems.append(_items.at(2));

	setCurrentItem(_items.at(0));
}

void BattleAbilitySelection::createAbilities() {
	if (_abilities.size() > 0) {
		_state = BattleAbilitySelection::ABILITYSELECTION;
		_action = BattleAction::ABILITY;
		_messageItem->setPlainText("Select an ability:");

		for (int i = 0; i < _items.size(); i++)
			_items.at(i)->hide();

		_activeItems.clear();
		for (int i = 0; i < qMin<int>(_items.size(), _abilities.size()); i++) {
			Ability *ability = _abilities.at(i);
			_items.at(i)->setPlainText(ability->getName() + "\t" + QString::number(ability->getCost()) + " " + _currentBattler->getEnergy().first->getName());
			_items.at(i)->show();
			_activeItems.append(_items.at(i));
		}

		_abilityIndex = 0;
		setCurrentItem(_items.at(0));
	}
}

void BattleAbilitySelection::nextItem() {
	if (_state == BattleAbilitySelection::ACTIONSELECTION) {
		if (_currentItem == _activeItems.at(_activeItems.size()-1))
			setCurrentItem(_activeItems.at(0));
		else
			setCurrentItem(_activeItems.at(_activeItems.indexOf(_currentItem)+1));
	} else if (_state == BattleAbilitySelection::ABILITYSELECTION) {
		if (_currentItem == _activeItems.last()) {

			// If the last ability was selected, scroll back to the first one
			if (_abilityIndex == (_abilities.size()-1)) {
				createAbilities();
			} else { /* Else scroll to the next ability */
				for (int i = 0; i < _activeItems.size()-1; i++) {
					QGraphicsTextItem *item = _activeItems.at(i);
					item->setPlainText(_activeItems.at(i+1)->toPlainText());
				}

				_abilityIndex++;
				Ability *ability = _abilities.at(_abilityIndex);

				QGraphicsTextItem *item = _activeItems.at(_activeItems.size()-1);
				item->setPlainText(ability->getName() + "\t" + QString::number(ability->getCost()) + " " + _currentBattler->getEnergy().first->getName());
			}
		} else {
			_abilityIndex++;
			setCurrentItem(_activeItems.at(_activeItems.indexOf(_currentItem)+1));
		}
	}
}

void BattleAbilitySelection::previousItem() {
	if (_state == BattleAbilitySelection::ACTIONSELECTION) {
		if (_currentItem == _activeItems.at(0))
			setCurrentItem(_activeItems.at(_activeItems.size()-1));
		else
			setCurrentItem(_activeItems.at(_activeItems.indexOf(_currentItem)-1));
	} else if (_state == BattleAbilitySelection::ABILITYSELECTION) {
		if (_currentItem == _activeItems.at(0)) {

			// If the first ability was selected, scroll to the last one
			if (_abilityIndex == 0) {
				for (int i = (_activeItems.size()-1); i >= 0 ; i--) {
					QGraphicsTextItem *item = _activeItems.at(i);
					Ability *ability = _abilities.at(_abilities.size() - _activeItems.size() + i);
					item->setPlainText(ability->getName() + "\t" + QString::number(ability->getCost()) + " " + _currentBattler->getEnergy().first->getName());
				}

				_abilityIndex = _abilities.size()-1;
				setCurrentItem(_activeItems.at(_activeItems.size()-1));
			} else { // Else scroll to the previous ability
				for (int i = (_activeItems.size()-1); i > 0; i--) {
					QGraphicsTextItem *item = _activeItems.at(i);
					item->setPlainText(_activeItems.at(i-1)->toPlainText());
				}

				_abilityIndex--;
				Ability *ability = _abilities.at(_abilityIndex);

				QGraphicsTextItem *item = _activeItems.at(0);
				item->setPlainText(ability->getName() + "\t" + QString::number(ability->getCost()) + " " + _currentBattler->getEnergy().first->getName());
			}
		} else {
			_abilityIndex--;
			setCurrentItem(_activeItems.at(_activeItems.indexOf(_currentItem)-1));
		}
	}
}

void BattleAbilitySelection::itemSelected() {
	if (_state == BattleAbilitySelection::ACTIONSELECTION) {
		if (_currentItem == _items.at(0)) {
			_action = BattleAction::ATTACK;
			emit actionSelected(BattleAction::ATTACK);
		} else if (_currentItem == _items.at(1)) {
			_action = BattleAction::ABILITY;
			createAbilities();
		} else if (_currentItem == _items.at(2)) {
			_action = BattleAction::FLEE;
			emit actionSelected(BattleAction::FLEE);
		}
	} else if (_state == BattleAbilitySelection::ABILITYSELECTION) {
		if (_abilityIndex < _abilities.size())
			emit actionSelected(BattleAction::ABILITY);
	}
}

void BattleAbilitySelection::setCurrentItem(QGraphicsTextItem *item) {

	if (!!_currentItem)
		_currentItem->setDefaultTextColor(Qt::white);

	_currentItem = item;
	_currentItem->setDefaultTextColor(Qt::yellow);
}