#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/BattleTargetSelection.h"

using namespace playmodule;

BattleTargetSelection::BattleTargetSelection(QGraphicsItem *parent, BattleTargetPointer *pointer)
	: QGraphicsPixmapItem(parent), _pointer(pointer), _battlers(0), _currentTarget(0),
	_items(0), _activeItems(0), _messageItem(0), _currentItem(0) {

		createWindow();

		QFont font ("Times", 12, QFont::Bold);
		QBrush brush(Qt::white);

		_messageItem = new QGraphicsSimpleTextItem(this);
		_messageItem->setText("Select a target:");
		_messageItem->setBrush(brush);
		_messageItem->setFont(font);
		_messageItem->setPos(25, 10);

		int x = 25, y = 40;
		for (int i = 0; i < 4; i++) {
			QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(this);
			item->hide();
			item->setBrush(brush);
			item->setFont(font);
			item->setPos(x, y);
			y += 30;

			_items.append(item);
		}
}

BattleTargetSelection::~BattleTargetSelection() {}

BattleItem* BattleTargetSelection::getTarget() const {
	return _battlers.at(_currentTarget);
}

void BattleTargetSelection::setTarget(BattleItem *item) {
	while (_battlers.at(_currentTarget) != item)
		nextItem();
}

void BattleTargetSelection::setBattlers(QVector<BattleCharacterItem*> characters, QVector<BattleEnemyItem*> enemies) {
	for (int i = 0; i < _items.size(); i++)
		_items.at(i)->hide();

	_activeItems.clear();
	_battlers.clear();

	for (int i = 0; i < characters.size(); i++)
		_battlers.append(characters.at(i));

	for (int i = 0; i < enemies.size(); i++)
		_battlers.append(enemies.at(i));

	for (int i = 0; i < qMin<int>(_battlers.size(), _items.size()); i++) {
		QGraphicsSimpleTextItem *item = _items.at(i);
		BattleItem *battler = _battlers.at(i);

		item->setText(battler->getName());
		item->show();
		_activeItems.append(item);
	}

	_currentTarget = 0;
	setCurrentItem(_items.at(0));
}

void BattleTargetSelection::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
		itemSelected();
		break;
	case Qt::Key_Down:
		nextItem();
		break;
	case Qt::Key_Up:
		previousItem();
		break;
	}
}

void BattleTargetSelection::createWindow() {
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

void BattleTargetSelection::nextItem() {
	if (_currentItem == _activeItems.last()) {
		// If the last battler was selected, scroll back to the first one
		if (_currentTarget == (_battlers.size()-1)) {
			for (int i = 0; i < _activeItems.size(); i++) {
				QGraphicsSimpleTextItem *item = _activeItems.at(i);
				BattleItem *battler = _battlers.at(i);
				item->setText(battler->getName());
			}

			_currentTarget = 0;
			setCurrentItem(_items.at(0));

		} else { // Else scroll to the next battler
			for (int i = 0; i < _activeItems.size()-1; i++) {
				QGraphicsSimpleTextItem *item = _activeItems.at(i);
				item->setText(_activeItems.at(i+1)->text());
			}

			_currentTarget++;
			BattleItem *battler = _battlers.at(_currentTarget);

			QGraphicsSimpleTextItem *item = _activeItems.last();
			item->setText(battler->getName());
			setCurrentItem(_currentItem);
		}
	} else {
		_currentTarget++;
		setCurrentItem(_activeItems.at(_activeItems.indexOf(_currentItem)+1));
	}
}

void BattleTargetSelection::previousItem() {
	if (_currentItem == _activeItems.first()) {

		// If the first battler was selected, scroll to the last one
		if (_currentTarget == 0) {
			for (int i = (_activeItems.size()-1); i >= 0; i--) {
				QGraphicsSimpleTextItem *item = _activeItems.at(i);
				BattleItem *battler = _battlers.at(_battlers.size() - _activeItems.size() + i);
				item->setText(battler->getName());
			}

			_currentTarget = _battlers.size()-1;
			setCurrentItem(_activeItems.last());

		} else { // Else scroll to the previous battler
			for (int i = (_activeItems.size()-1); i > 0; i--) {
				QGraphicsSimpleTextItem *item = _activeItems.at(i);
				item->setText(_activeItems.at(i-1)->text());
			}

			_currentTarget--;
			BattleItem *battler = _battlers.at(_currentTarget);

			QGraphicsSimpleTextItem *item = _activeItems.first();
			item->setText(battler->getName());
			setCurrentItem(_currentItem);
		}

	} else {
		_currentTarget--;
		setCurrentItem(_activeItems.at(_activeItems.indexOf(_currentItem)-1));
	}
}

void BattleTargetSelection::itemSelected() {
	emit targetSelected(_battlers.at(_currentTarget));
}

void BattleTargetSelection::setCurrentItem(QGraphicsSimpleTextItem *item) {
	QBrush whiteBrush(Qt::white);
	QBrush yellowBrush(Qt::yellow);

	if (!!_currentItem)
		_currentItem->setBrush(whiteBrush);

	_currentItem = item;
	_currentItem->setBrush(yellowBrush);

	_pointer->setTarget(_battlers.at(_currentTarget));
}