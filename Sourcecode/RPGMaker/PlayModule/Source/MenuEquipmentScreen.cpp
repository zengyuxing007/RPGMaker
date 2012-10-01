#include "qpoint.h"
#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/MenuEquipmentScreen.h"

using namespace playmodule;

MenuEquipmentScreen::MenuEquipmentScreen(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _state(EQUIPPED_SELECTION), _inventory(0), _character(0),
	_currentEquippedEquipmentPiece(0), _currentEquippedEquipmentItem(0),
	_currentSelectedEquipmentString(0), _currentEquipmentListItem(0) {

		createWindow();
		createImagePanel();
		createLabels();
}

MenuEquipmentScreen::~MenuEquipmentScreen() {}

void MenuEquipmentScreen::initialize(BattleCharacterItem *character, Inventory *inventory) {
	QFont font("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);
	_state = EQUIPPED_SELECTION;

	// Set character and inventory
	_character = character;
	_inventory = inventory;

	// Set the character's equipment
	QPointF position(_equippedEquipmentItem->pos().x(), _equippedEquipmentItem->pos().y() + 40);

	QStringList equipmentHeaders;
	equipmentHeaders << "Main Hand: " << "Off Hand: "
		<< "Helmet: " << "Torso: " << "Leggings: " << "Gloves: " << "Boots: "
		<< "Earring: " << "Necklace: " << "Ring: ";
	QVector<EquipmentPiece*> equipment = _character->getEquipment()->getEquipment();
	_currentEquippedEquipmentPiece = equipment.at(0);
	_equippedEquipmentItem->setText("Equipment of " + _character->getName());

	for (int i = 0; i < equipment.size(); i++) {
		QString itemText;
		itemText.append(equipmentHeaders.at(i));
		
		if (!!equipment.at(i))
			itemText.append(equipment.at(i)->getName());

		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(this);
		item->setText(itemText);
		item->setBrush(brush);
		item->setFont(font);
		item->setPos(position);

		_equippedEquipmentItems.append(item);
		position.setY(position.y() + 20);
	}

	setCurrentEquippedItem(_equippedEquipmentItems.at(0), equipment.at(0));

	// Set Image
	if (_character->getCharacter()->getBattleImage().second != QString()) {
		QPixmap battlerImage = _character->getCharacter()->getBattleImage().first;

		if ((battlerImage.width() > _characterImage->boundingRect().width()) || (battlerImage.height() > _characterImage->boundingRect().height()))
			battlerImage = battlerImage.scaled(QSize(200, 200), Qt::KeepAspectRatio);
		
		QPixmap frame(200, 200);
		frame.fill(Qt::transparent);
		QPainter painter(&frame);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

		QPoint position(0, 0);
		position.setX(frame.width()/2 - battlerImage.width()/2);
		position.setY(frame.height()/2 - battlerImage.height()/2);
		painter.drawPixmap(position, battlerImage);
		painter.end();

		_characterImage->setPixmap(frame);
		
	} else {
		QPixmap image(200, 200);
		image.fill(Qt::transparent);
		_characterImage->setPixmap(image);
	}
}

void MenuEquipmentScreen::finalize() {
	_inventory = 0;
	_character = 0;
	_characterImage->setPixmap(QPixmap(200, 200));
	
	_currentEquippedEquipmentPiece = 0;
	_currentEquippedEquipmentItem = 0;
	_equippedEquipmentItem->setText(QString());
	for (int i = 0; i < _equippedEquipmentItems.size(); i++)
		delete _equippedEquipmentItems.at(i);
	_equippedEquipmentItems.clear();

	_equippedStatsItem->setText(QString());
	for (int i = 0; i < _equippedStatsItems.size(); i++)
		delete _equippedStatsItems.at(i);
	_equippedStatsItems.clear();

	_updatedStatsItem->setText(QString());
	for (int i = 0; i < _updatedStatsItems.size(); i++)
		delete _updatedStatsItems.at(i);
	_updatedStatsItems.clear();

	_equipmentList.clear();
	_equipmentStringList.clear();
	_currentSelectedEquipmentString = 0;
	_currentEquipmentListItem = 0;
	_equipmentListItem->setText(QString());
	for (int i = 0; i < _activeEquipmentListItems.size(); i++)
		delete _activeEquipmentListItems.at(i);
	_activeEquipmentListItems.clear();
}

BattleCharacterItem* MenuEquipmentScreen::getCharacter() const {
	return _character;
}

void MenuEquipmentScreen::keyPressEvent(QKeyEvent *event) {
	if (_state == MenuEquipmentScreen::EQUIPPED_SELECTION) {
		switch (event->key()) {
		case Qt::Key_Down:
			nextEquippedItem();
			break;
		case Qt::Key_Up:
			previousEquippedItem();
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			equippedItemSelected();
			break;
		case Qt::Key_Escape:
		case Qt::Key_Backspace:
			emit closeCharacterEquipment();
			break;
		}
	} else if (_state == MenuEquipmentScreen::EQUIPMENT_SELECTION) {
		switch (event->key()) {
		case Qt::Key_Down:
			nextEquipmentItem();
			break;
		case Qt::Key_Up:
			previousEquipmentItem();
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			equipmentItemSelected();
			break;
		case Qt::Key_Escape:
		case Qt::Key_Backspace:

			_updatedStatsItem->setText(QString());
			for (int i = 0; i < _updatedStatsItems.size(); i++)
				delete _updatedStatsItems.at(i);
			_updatedStatsItems.clear();

			_equipmentList.clear();
			_equipmentStringList.clear();
			_currentSelectedEquipmentString = 0;
			_currentEquipmentListItem = 0;
			_equipmentListItem->setText(QString());
			for (int i = 0; i < _activeEquipmentListItems.size(); i++)
				delete _activeEquipmentListItems.at(i);
			_activeEquipmentListItems.clear();

			_state = MenuEquipmentScreen::EQUIPPED_SELECTION;
			break;
		}
	}
}

void MenuEquipmentScreen::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MenuEquipmentScreen::createWindow() {
	// Get and resize the panel
	QPixmap windowSkin(QPixmap(g_windowskinPath + "001-Blue01.png"));
	QPixmap panel(windowSkin.copy(0, 0, 128, 128));
	panel = panel.scaled(QSize(800, 600));

	setPixmap(panel);
	panel.fill(Qt::transparent);

	// Paint the panel's border
	QPainter painter(&panel);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	QPixmap borderTile(16, 16);

	// Northern border
	borderTile = windowSkin.copy(152, 0, 16, 16);
	for (int i = 1; i < panel.width()/16; i++)
		painter.drawPixmap(borderTile.width() * i, 0, borderTile);

	// Southern border
	borderTile = windowSkin.copy(152, 48, 16, 16);
	for (int i = 1; i < panel.width()/16; i++)
		painter.drawPixmap(borderTile.width() * i, panel.height() - borderTile.height(), borderTile);

	// Western border
	borderTile = windowSkin.copy(128, 24, 16, 16);
	for (int i = 1; i < panel.height()/16; i++)
		painter.drawPixmap(0, borderTile.height() * i, borderTile);

	// Eastern border
	borderTile = windowSkin.copy(176, 24, 16, 16);
	for (int i = 1; i < panel.height()/16; i++)
		painter.drawPixmap(panel.width() - borderTile.width(), borderTile.height() * i, borderTile);

	// North-West corner
	borderTile = windowSkin.copy(128, 0, 16, 16);
	painter.drawPixmap(0, 0, borderTile);

	// North-East corner
	borderTile = windowSkin.copy(176, 0, 16, 16);
	painter.drawPixmap(panel.width()-16, 0, borderTile);

	// South-West corner
	borderTile = windowSkin.copy(128, 48, 16, 16);
	painter.drawPixmap(0, panel.height()-16, borderTile);

	// South-East corner
	borderTile = windowSkin.copy(176, 48, 16, 16);
	painter.drawPixmap(panel.width()-16, panel.height()-16, borderTile);

	_equippedEquipmentPanel = new QGraphicsPixmapItem(this);
	_equippedEquipmentPanel->setPixmap(panel.scaled(800, 325));
	_equippedEquipmentPanel->setPos(0, 0);

	_equipmentStatsPanel = new QGraphicsPixmapItem(this);
	_equipmentStatsPanel->setPixmap(panel.scaled(267, 275));
	_equipmentStatsPanel->setPos(0, _equippedEquipmentPanel->boundingRect().height());
	
	_updatedStatsPanel = new QGraphicsPixmapItem(this);
	_updatedStatsPanel->setPixmap(panel.scaled(266, 275));
	_updatedStatsPanel->setPos(_equipmentStatsPanel->pos().x() + _equipmentStatsPanel->boundingRect().width(), _equippedEquipmentPanel->boundingRect().height());

	_equipmentListPanel = new QGraphicsPixmapItem(this);
	_equipmentListPanel->setPixmap(panel.scaled(267, 275));
	_equipmentListPanel->setPos(_updatedStatsPanel->pos().x() + _updatedStatsPanel->boundingRect().width(), _equippedEquipmentPanel->boundingRect().height());
}

void MenuEquipmentScreen::createImagePanel() {
	_characterImage = new QGraphicsPixmapItem(this);
	QPixmap characterImage(200, 200);
	characterImage.fill(Qt::transparent);
	_characterImage->setPixmap(characterImage);
	_characterImage->setPos(50, 50);
}

void MenuEquipmentScreen::createLabels() {
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	_equippedEquipmentItem = new QGraphicsSimpleTextItem(_equippedEquipmentPanel);
	_equippedEquipmentItem->setPos(300, 10);
	_equippedEquipmentItem->setBrush(brush);
	_equippedEquipmentItem->setFont(font);

	_equippedStatsItem = new QGraphicsSimpleTextItem(_equipmentStatsPanel);
	_equippedStatsItem->setPos(25, 10);
	_equippedStatsItem->setBrush(brush);
	_equippedStatsItem->setFont(font);

	_updatedStatsItem = new QGraphicsSimpleTextItem(_updatedStatsPanel);
	_updatedStatsItem->setPos(25, 10);
	_updatedStatsItem->setBrush(brush);
	_updatedStatsItem->setFont(font);

	_equipmentListItem = new QGraphicsSimpleTextItem(_equipmentListPanel);
	_equipmentListItem->setPos(25, 10);
	_equipmentListItem->setBrush(brush);
	_equipmentListItem->setFont(font);
}

void MenuEquipmentScreen::nextEquippedItem() {
	if (_currentEquippedEquipmentItem == _equippedEquipmentItems.last())
		setCurrentEquippedItem(_equippedEquipmentItems.first(), _character->getEquipment()->getEquipment().first());
	else {
		QGraphicsSimpleTextItem *item = _equippedEquipmentItems.at(_equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem)+1);
		QVector<EquipmentPiece*> equipment = _character->getEquipment()->getEquipment();

		setCurrentEquippedItem(item, equipment.at(_equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem)+1));
	}
}

void MenuEquipmentScreen::previousEquippedItem() {
	if (_currentEquippedEquipmentItem == _equippedEquipmentItems.first())
		setCurrentEquippedItem(_equippedEquipmentItems.last(), _character->getEquipment()->getEquipment().last());
	else {
		QGraphicsSimpleTextItem *item = _equippedEquipmentItems.at(_equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem)-1);
		QVector<EquipmentPiece*> equipment = _character->getEquipment()->getEquipment();

		setCurrentEquippedItem(item, equipment.at(_equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem)-1));
	}
}

void MenuEquipmentScreen::equippedItemSelected() {
	// Reset updated stats and equipment list
	_updatedStatsItem->setText(QString());
	for (int i = 0; i < _updatedStatsItems.size(); i++)
		delete _updatedStatsItems.at(i);
	_updatedStatsItems.clear();

	_equipmentList.clear();
	_equipmentStringList.clear();
	_currentSelectedEquipmentString = 0;
	_currentEquipmentListItem = 0;
	_equipmentListItem->setText(QString());
	for (int i = 0; i < _activeEquipmentListItems.size(); i++)
		delete _activeEquipmentListItems.at(i);
	_activeEquipmentListItems.clear();

	// Build the equipmentlist
	QVector<QPair<EquipmentPiece*, int>> equipment = _inventory->getEquipment();
	for (int i = 0; i < equipment.size();) {
		if (!equipmentFilter(equipment.at(i).first))
			equipment.remove(i);
		else {
			_equipmentList.append(equipment.at(i).first);
			i++;
		}
	}

	// Set the equipment list
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);
	QPointF position(_equipmentListItem->pos().x(), _equipmentListItem->pos().y() + 20);

	_currentSelectedEquipmentString = 0;
	_equipmentStringList.append(QString("Clear equipment slot"));
	for (int i = 0; i < _equipmentList.size(); i++)
		_equipmentStringList.append(_equipmentList.at(i)->getName());

	_equipmentListItem->setText("Select equipment:");
	for (int i = 0; i < qMin<int>(_equipmentStringList.size(), 8); i++) {
		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(_equipmentListPanel);

		item->setBrush(brush);
		item->setFont(font);
		item->setText(_equipmentStringList.at(i));
		item->setPos(position);

		position.setY(position.y() + 20);
		_activeEquipmentListItems.append(item);
	}

	setCurrentEquipmentItem(_activeEquipmentListItems.first(), 0);
	_state = MenuEquipmentScreen::EQUIPMENT_SELECTION;
}

void MenuEquipmentScreen::nextEquipmentItem() {
	if (_currentEquipmentListItem == _activeEquipmentListItems.last()) {
		// If the last equipment item was selected
		if (_currentSelectedEquipmentString == (_equipmentStringList.size()-1)) {

			for (int i = 0; i < _activeEquipmentListItems.size(); i++) {
				QGraphicsSimpleTextItem *item = _activeEquipmentListItems.at(i);
				item->setText(_equipmentStringList.at(i));
			}

			_currentSelectedEquipmentString = 0;
			setCurrentEquipmentItem(_activeEquipmentListItems.at(0), 0);

		} else { // Else scroll to the next equipment item

			for (int i = 0; i < _activeEquipmentListItems.size()-1; i++) {
				QGraphicsSimpleTextItem *item = _activeEquipmentListItems.at(i);
				item->setText(_activeEquipmentListItems.at(i+1)->text());
			}

			_currentSelectedEquipmentString++;
			QGraphicsSimpleTextItem *item = _activeEquipmentListItems.at(_activeEquipmentListItems.size() - 1);
			item->setText(_equipmentStringList.at(_currentSelectedEquipmentString));
			setCurrentEquipmentItem(item, _equipmentList.at(_currentSelectedEquipmentString - 1));
		}

	} else {
		_currentSelectedEquipmentString++;
		setCurrentEquipmentItem(_activeEquipmentListItems.at(_activeEquipmentListItems.indexOf(_currentEquipmentListItem) + 1), _equipmentList.at(_currentSelectedEquipmentString -1));
	}
}

void MenuEquipmentScreen::previousEquipmentItem() {
	if (_currentEquipmentListItem == _activeEquipmentListItems.first()) {
		
		// If the first equipment item was selected
		if (_currentSelectedEquipmentString == 0) {
			for (int i = (_activeEquipmentListItems.size()-1); i >= 0; i--) {
				QGraphicsSimpleTextItem *item = _activeEquipmentListItems.at(i);
				item->setText(_equipmentStringList.at(_equipmentStringList.size() - _activeEquipmentListItems.size() + i));
			}

			_currentSelectedEquipmentString = _equipmentStringList.size() - 1;
			if (!_equipmentList.empty())
				setCurrentEquipmentItem(_activeEquipmentListItems.last(), _equipmentList.last());
			else
				setCurrentEquipmentItem(_activeEquipmentListItems.last(), 0);

		} else { // Else scroll to the previous equipment item
			for (int i = (_activeEquipmentListItems.size() - 1); i > 0; i--) {
				QGraphicsSimpleTextItem *item = _activeEquipmentListItems.at(i);
				item->setText(_activeEquipmentListItems.at(i-1)->text());
			}

			_currentSelectedEquipmentString--;
			QGraphicsSimpleTextItem *item = _activeEquipmentListItems.first();
			item->setText(_equipmentStringList.at(_currentSelectedEquipmentString));
			setCurrentEquipmentItem(item, _equipmentList.at(_currentSelectedEquipmentString - 1));
		}

	} else {
		_currentSelectedEquipmentString--;
		if (_currentSelectedEquipmentString == 0)
			setCurrentEquipmentItem(_activeEquipmentListItems.at(_activeEquipmentListItems.indexOf(_currentEquipmentListItem) - 1), 0);
		else
			setCurrentEquipmentItem(_activeEquipmentListItems.at(_activeEquipmentListItems.indexOf(_currentEquipmentListItem) - 1), _equipmentList.at(_currentSelectedEquipmentString -1));
	}
}

void MenuEquipmentScreen::equipmentItemSelected() {
	// Remove equipment piece from character and add to the inventory
	if (!!_currentEquippedEquipmentPiece) {
		_inventory->addEquipment(_currentEquippedEquipmentPiece, 1);
		_character->getEquipment()->removeEquipment(_currentEquippedEquipmentPiece);
	}

	// If the player added an equipment item
	if (_currentSelectedEquipmentString != 0) {
		_character->getEquipment()->setEquipment(_equipmentList.at(_currentSelectedEquipmentString - 1));
		_inventory->takeEquipment(_equipmentList.at(_currentSelectedEquipmentString - 1)->getName(), 1);
	}


	BattleCharacterItem *character = _character;
	Inventory *inventory = _inventory;
	int currentEquippedItemIndex = _equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem);

	finalize();
	initialize(character, inventory);
	setCurrentEquippedItem(_equippedEquipmentItems.at(currentEquippedItemIndex), _character->getEquipment()->getEquipment().at(currentEquippedItemIndex));
}

void MenuEquipmentScreen::setCurrentEquippedItem(QGraphicsSimpleTextItem *item, EquipmentPiece *equipment) {
	QBrush whiteBrush(Qt::white);
	QBrush yellowBrush(Qt::yellow);
	QFont font("Times", 12, QFont::Bold);

	if (!!_currentEquippedEquipmentItem)
		_currentEquippedEquipmentItem->setBrush(whiteBrush);

	_currentEquippedEquipmentItem = item;
	_currentEquippedEquipmentItem->setBrush(yellowBrush);
	_currentEquippedEquipmentPiece = equipment;

	// Reset the current selected equipped item
	_equippedStatsItem->setText(QString());
	for (int i = 0; i < _equippedStatsItems.size(); i++)
		delete _equippedStatsItems.at(i);
	_equippedStatsItems.clear();

	// If the current slot contains an equipment piece
	if (!!_currentEquippedEquipmentPiece) {
		QPointF position(_equippedStatsItem->pos().x(), _equippedStatsItem->pos().y() + 20);
		_equippedStatsItem->setText("Attributes of " + equipment->getName());
		QVector<QPair<Attribute*, int>> attributes = equipment->getAttributes();

		for (int i = 0; i < attributes.size(); i++) {
			QPair<Attribute*, int> it = attributes.at(i);
			QString itemText(it.first->getName() + " + " + QString::number(it.second));

			QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(_equipmentStatsPanel);

			item->setText(itemText);
			item->setBrush(whiteBrush);
			item->setFont(font);
			item->setPos(position);

			_equippedStatsItems.append(item);
			position.setY(position.y() + 20);
		}

	} else
		_equippedStatsItem->setText("No equipment selected");
}

void MenuEquipmentScreen::setCurrentEquipmentItem(QGraphicsSimpleTextItem *item, EquipmentPiece *equipment) {
	// Set brushes
	QBrush whiteBrush(Qt::white);
	QBrush yellowBrush(Qt::yellow);
	QBrush greenBrush(Qt::green);
	QBrush redBrush(Qt::red);
	QFont font("Times", 12, QFont::Bold);

	// Set current item
	if (!!_currentEquipmentListItem)
		_currentEquipmentListItem->setBrush(whiteBrush);

	_currentEquipmentListItem = item;
	_currentEquipmentListItem->setBrush(yellowBrush);

	// Reset the updated equipment stats
	_updatedStatsItem->setText(QString());
	for (int i = 0; i < _updatedStatsItems.size(); i++)
		delete _updatedStatsItems.at(i);
	_updatedStatsItems.clear();

	// Set the updated equipment stats
	QPointF position(_updatedStatsItem->pos().x(), _updatedStatsItem->pos().y() + 20);
	_updatedStatsItem->setText("Preview:");

	QVector<QPair<Attribute*, int>> currentEquipped(0);
	if (!!_currentEquippedEquipmentPiece)
		currentEquipped = _currentEquippedEquipmentPiece->getAttributes();

	QVector<QPair<Attribute*, int>> previewEquipped(0);
	if (!!equipment)
		previewEquipped = equipment->getAttributes();

	// First go over all of the currently equipped 
	// attributes for comparison.
	for (int i = 0; i < currentEquipped.size(); i++) {
		QPair<Attribute*, int> it1 = currentEquipped.at(i);
		QPair<Attribute*, int> it2(0, 0);

		for (int j = 0; j < previewEquipped.size();) {
			if (previewEquipped.at(j).first == it1.first) {
				it2 = previewEquipped.at(j);
				previewEquipped.remove(j);
				break;
			} else {
				j++;
			}
		}

		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(_updatedStatsPanel);
		item->setText(it1.first->getName() + " + " + QString::number(it2.second));
		item->setFont(font);
		item->setPos(position);

		// Set brush
		if (it1.second > it2.second)
			item->setBrush(redBrush);
		else if (it1.second < it2.second)
			item->setBrush(greenBrush);
		else
			item->setBrush(whiteBrush);

		position.setY(position.y() + 20);
		_updatedStatsItems.append(item);
	}

	// Go over all the attributes of the considered
	// Equipment Piece for comparison.
	for (int i = 0; i < previewEquipped.size(); i++) {
		QPair<Attribute*, int> it = previewEquipped.at(i);

		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(_updatedStatsPanel);
		item->setText(it.first->getName() + " + " + QString::number(it.second));
		item->setFont(font);
		item->setBrush(greenBrush);
		item->setPos(position);

		position.setY(position.y() + 20);
		_updatedStatsItems.append(item);
	}
}

bool MenuEquipmentScreen::equipmentFilter(EquipmentPiece *equipment) {
	// Filter by type (Weapon, Armor or Accessory)
	EquipmentPiece::Type typeFilter;
	if (_equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem) < 2)
		typeFilter = EquipmentPiece::WEAPON;
	else if (_equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem) < 7)
		typeFilter = EquipmentPiece::ARMOR;
	else
		typeFilter = EquipmentPiece::ACCESSORY;

	if (equipment->getType() != typeFilter)
		return false;

	// Filter by subtype
	if (typeFilter == EquipmentPiece::WEAPON) {
		Weapon *weapon = dynamic_cast<Weapon*>(equipment);
		int subTypeFilter = _equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem);

		if ((weapon->getSubtype() != Weapon::TWOHANDED) && (subTypeFilter != weapon->getSubtype()))
			return false;

	} else if (typeFilter == EquipmentPiece::ARMOR) {
		Armor *armor = dynamic_cast<Armor*>(equipment);
		int subTypeFilter = _equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem);

		if (subTypeFilter != armor->getSubtype())
			return false;

	} else if (typeFilter == EquipmentPiece::ACCESSORY) {
		Accessory *accessory = dynamic_cast<Accessory*>(equipment);
		int subTypeFilter = _equippedEquipmentItems.indexOf(_currentEquippedEquipmentItem);

		if (subTypeFilter != accessory->getSubtype())
			return false;
	}

	QVector<QPair<Class*, int>> classes = equipment->getClasses();
	for (int i = 0; i < classes.size(); i++) {
		QPair<Class*, int> it = classes.at(i);
		if ((it.first == _character->getClass()) && (it.second <= _character->getLevel()))
			return true;
	}

	return false;
}