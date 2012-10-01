#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/MenuInventoryScreen.h"

using namespace playmodule;

MenuInventoryScreen::MenuInventoryScreen(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _currentItem(0), _currentItemLabel(0), _inventory(0) {
		
		createWindow();
		createLabels();
}

MenuInventoryScreen::~MenuInventoryScreen() {}

void MenuInventoryScreen::initialize(Inventory *inventory) {
	QFont font("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	// Set inventory
	_inventory = inventory;

	// Build items list
	QPointF position(_itemsLabel->pos().x(), _itemsLabel->pos().y() + 40);
	_itemsLabel->setText("Inventory:");

	QVector<QPair<EquipmentPiece*, int>> items = _inventory->getEquipment();
	for (int i = 0; i < qMin<int>(items.size(), 25); i++) {
		QPair<EquipmentPiece*, int> it = items.at(i);
		QString itemText;
		itemText.append(it.first->getName());
		itemText.append(" (" + QString::number(it.second) + ")");

		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(_itemsPanel);
		item->setText(itemText);
		item->setBrush(brush);
		item->setFont(font);
		item->setPos(position);

		_activeItemLabels.append(item);
		position.setY(position.y() + 20);
	}

	// Set current item
	if (!items.empty()) {
		_currentItem = 0;
		setCurrentItem(_activeItemLabels.first());
	} else {
		_currentItem = -1;
	}

}

void MenuInventoryScreen::finalize() {
	_inventory = 0;

	_currentItem = 0;
	_itemsLabel->setText(QString());
	_currentItemLabel = 0;
	for (int i = 0; i < _activeItemLabels.size(); i++)
		delete _activeItemLabels.at(i);
	_activeItemLabels.clear();

	_nameLabel->setText(QString());
	_typeLabel->setText(QString());
	_descriptionLabel->setText(QString());
	_description->setPlainText(QString());

	_classLabel->setText(QString());
	for (int i = 0; i < _classLabels.size(); i++)
		delete _classLabels.at(i);
	_classLabels.clear();

	_elementLabel->setText(QString());
	for (int i = 0; i < _elementLabels.size(); i++)
		delete _elementLabels.at(i);
	_elementLabels.clear();

	_attributeLabel->setText(QString());
	for (int i = 0; i < _attributeLabels.size(); i++)
		delete _attributeLabels.at(i);
	_attributeLabels.clear();
}

void MenuInventoryScreen::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Down:
		nextItem();
		break;
	case Qt::Key_Up:
		previousItem();
		break;
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		emit closeInventory();
		break;
	}
}

void MenuInventoryScreen::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MenuInventoryScreen::createWindow() {
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

	_itemsPanel = new QGraphicsPixmapItem(this);
	_itemsPanel->setPixmap(panel.scaled(200, 600));
	_itemsPanel->setPos(0, 0);

	_descriptionPanel = new QGraphicsPixmapItem(this);
	_descriptionPanel->setPixmap(panel.scaled(600, 300));
	_descriptionPanel->setPos(_itemsPanel->boundingRect().width(), 0);

	_classPanel = new QGraphicsPixmapItem(this);
	_classPanel->setPixmap(panel.scaled(200, 300));
	_classPanel->setPos(_descriptionPanel->pos().x(), _descriptionPanel->pos().y() + _descriptionPanel->boundingRect().height());

	_elementPanel = new QGraphicsPixmapItem(this);
	_elementPanel->setPixmap(panel.scaled(200, 300));
	_elementPanel->setPos(_classPanel->pos().x() + _classPanel->boundingRect().width(), _descriptionPanel->pos().y() + _descriptionPanel->boundingRect().height());

	_attributePanel = new QGraphicsPixmapItem(this);
	_attributePanel->setPixmap(panel.scaled(200, 300));
	_attributePanel->setPos(_elementPanel->pos().x() + _elementPanel->boundingRect().width(), _descriptionPanel->pos().y() + _descriptionPanel->boundingRect().height());
}

void MenuInventoryScreen::createLabels() {
	QFont font("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	// Items
	_itemsLabel = new QGraphicsSimpleTextItem(_itemsPanel);
	_itemsLabel->setBrush(brush);
	_itemsLabel->setFont(font);
	_itemsLabel->setPos(25, 10);

	// Description
	_nameLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_nameLabel->setBrush(brush);
	_nameLabel->setFont(font);
	_nameLabel->setPos(25, 10);

	_typeLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_typeLabel->setBrush(brush);
	_typeLabel->setFont(font);
	_typeLabel->setPos(25, 30);

	_descriptionLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_descriptionLabel->setBrush(brush);
	_descriptionLabel->setFont(font);
	_descriptionLabel->setPos(25, 50);

	_description = new QGraphicsTextItem(_descriptionPanel);
	_description->setTextWidth(_descriptionPanel->boundingRect().width() - 25);
	_description->setPos(20, 70);

	// Classes
	_classLabel = new QGraphicsSimpleTextItem(_classPanel);
	_classLabel->setBrush(brush);
	_classLabel->setFont(font);
	_classLabel->setPos(25, 10);

	// Elements
	_elementLabel = new QGraphicsSimpleTextItem(_elementPanel);
	_elementLabel->setBrush(brush);
	_elementLabel->setFont(font);
	_elementLabel->setPos(25, 10);

	// Attributes
	_attributeLabel = new QGraphicsSimpleTextItem(_attributePanel);
	_attributeLabel->setBrush(brush);
	_attributeLabel->setFont(font);
	_attributeLabel->setPos(25, 10);
}

void MenuInventoryScreen::nextItem() {
	if (_currentItem < 0)
		return;

	if (_currentItemLabel == _activeItemLabels.last()) {

		// If the last item was selected, scroll back to the first one
		if (_currentItem == (_inventory->getEquipment().size()-1)) {
			for (int i = 0; i < _activeItemLabels.size(); i++) {
				QGraphicsSimpleTextItem *item = _activeItemLabels.at(i);
				QPair<EquipmentPiece*, int> equipment = _inventory->getEquipment().at(i);
				item->setText(equipment.first->getName() + " (" + QString::number(equipment.second) + ")");
			}

			_currentItem = 0;
			setCurrentItem(_activeItemLabels.at(0));

		} else { // Else scroll to the next item
			for (int i = 0; i < _activeItemLabels.size() -1; i++) {
				QGraphicsSimpleTextItem *item = _activeItemLabels.at(i);
				item->setText(_activeItemLabels.at(i+1)->text());
			}

			_currentItem++;
			QPair<EquipmentPiece*, int> equipment = _inventory->getEquipment().at(_currentItem);

			QGraphicsSimpleTextItem *item = _activeItemLabels.last();
			item->setText(equipment.first->getName() + " (" + QString::number(equipment.second) + ")");
			setCurrentItem(item);
		}

	} else {
		_currentItem++;
		setCurrentItem(_activeItemLabels.at(_activeItemLabels.indexOf(_currentItemLabel)+1));
	}
}

void MenuInventoryScreen::previousItem() {
	if (_currentItem < 0)
		return;

	if (_currentItemLabel == _activeItemLabels.first()) {

		// If the first item was selected, scroll to the last one
		if (_currentItem == 0) {
			for (int i = (_activeItemLabels.size()-1); i >= 0; i--) {
				QGraphicsSimpleTextItem *item = _activeItemLabels.at(i);
				QPair<EquipmentPiece*, int> equipment = _inventory->getEquipment().at(_inventory->getEquipment().size() - _activeItemLabels.size() + i);
				item->setText(equipment.first->getName() + " (" + QString::number(equipment.second) + ")");
			}

			_currentItem = _inventory->getEquipment().size() -1;
			setCurrentItem(_activeItemLabels.last());

		} else { // Else scroll to the previous item

			for (int i = (_activeItemLabels.size()-1); i > 0; i--) {
				QGraphicsSimpleTextItem *item = _activeItemLabels.at(i);
				item->setText(_activeItemLabels.at(i-1)->text());
			}

			_currentItem--;
			QPair<EquipmentPiece*, int> equipment = _inventory->getEquipment().at(_currentItem);

			QGraphicsSimpleTextItem *item = _activeItemLabels.first();
			item->setText(equipment.first->getName() + " (" + QString::number(equipment.second) + ")");
			setCurrentItem(_currentItemLabel);
		}

	} else {
		_currentItem--;
		setCurrentItem(_activeItemLabels.at(_activeItemLabels.indexOf(_currentItemLabel) -1));
	}
}

void MenuInventoryScreen::setCurrentItem(QGraphicsSimpleTextItem *item) {
	QBrush whiteBrush(Qt::white);
	QBrush yellowBrush(Qt::yellow);
	QFont font("Times", 12, QFont::Bold);

	// Set current selected item
	if (!!_currentItemLabel)
		_currentItemLabel->setBrush(whiteBrush);

	_currentItemLabel = item;
	_currentItemLabel->setBrush(yellowBrush);

	// Get corresponding item
	EquipmentPiece *equipment = _inventory->getEquipment().at(_currentItem).first;

	// Clean the panels
	_classLabel->setText(QString());
	for (int i = 0; i < _classLabels.size(); i++)
		delete _classLabels.at(i);
	_classLabels.clear();

	_elementLabel->setText(QString());
	for (int i = 0; i < _elementLabels.size(); i++)
		delete _elementLabels.at(i);
	_elementLabels.clear();

	_attributeLabel->setText(QString());
	for (int i = 0; i < _attributeLabels.size(); i++)
		delete _attributeLabels.at(i);
	_attributeLabels.clear();

	// Fill in the description
	_nameLabel->setText("Name: " + equipment->getName());
	_typeLabel->setText("Type: ");

	switch (equipment->getType()) {
	case EquipmentPiece::WEAPON:
		{
			_typeLabel->setText(_typeLabel->text() + "Weapon ");
			Weapon *weapon = dynamic_cast<Weapon*>(equipment);
			switch (weapon->getSubtype()) {
			case Weapon::MAINHAND:
				_typeLabel->setText(_typeLabel->text() + "(Main hand)");
				break;
			case Weapon::OFFHAND:
				_typeLabel->setText(_typeLabel->text() + "(Off hand)");
				break;
			case Weapon::TWOHANDED:
				_typeLabel->setText(_typeLabel->text() + "(Two handed)");
				break;
			}
		}
		break;
	case EquipmentPiece::ARMOR:
		{
			_typeLabel->setText(_typeLabel->text() + "Armor ");
			Armor *armor = dynamic_cast<Armor*>(equipment);
			switch (armor->getSubtype()) {
			case Armor::HELMET:
				_typeLabel->setText(_typeLabel->text() + "(Headpiece)");
				break;
			case Armor::TORSO:
				_typeLabel->setText(_typeLabel->text() + "(Chestpiece)");
				break;
			case Armor::LEGGINGS:
				_typeLabel->setText(_typeLabel->text() + "(Leggings)");
				break;
			case Armor::GLOVES:
				_typeLabel->setText(_typeLabel->text() + "(Gloves)");
				break;
			case Armor::BOOTS:
				_typeLabel->setText(_typeLabel->text() + "(Boots)");
				break;
			}
		}
		break;
	case EquipmentPiece::ACCESSORY:
		{
			_typeLabel->setText(_typeLabel->text() + "Accessory ");
			Accessory *accessory = dynamic_cast<Accessory*>(equipment);
			switch (accessory->getSubtype()) {
			case Accessory::EARRING:
				_typeLabel->setText(_typeLabel->text() + "(Earring)");
				break;
			case Accessory::NECKLACE:
				_typeLabel->setText(_typeLabel->text() + "(Necklace)");
				break;
			case Accessory::RING:
				_typeLabel->setText(_typeLabel->text() + "(Ring)");
				break;
			}
		}
		break;
	}
	
	_descriptionLabel->setText("Description:");
	_description->setPlainText(equipment->getDescription());
	_description->setFont(font);
	_description->setDefaultTextColor(Qt::white);

	QTextBlockFormat format;
	format.setAlignment(Qt::AlignLeft);
	QTextCursor cursor = _description->textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(format);
	cursor.clearSelection();
	_description->setTextCursor(cursor);

	// Fill in the classes
	_classLabel->setText("Classes:");
	QPointF position(_classLabel->pos().x(), _classLabel->pos().y() + 40);
	QVector<QPair<Class*, int>> classes = equipment->getClasses();
	for (int i = 0; i < classes.size(); i++) {
		QPair<Class*, int> it = classes.at(i);
		QGraphicsSimpleTextItem *classItem = new QGraphicsSimpleTextItem(_classPanel);
		classItem->setBrush(whiteBrush);
		classItem->setFont(font);
		classItem->setPos(position);
		classItem->setText(it.first->getName() + " (Level " + QString::number(it.second) + ")");

		position.setY(position.y() + 20);
		_classLabels.append(classItem);
	}

	// Fill in the elements
	_elementLabel->setText("Elements:");
	position.setX(_elementLabel->pos().x());
	position.setY(_elementLabel->pos().y() + 40);
	QVector<Element*> elements = equipment->getElements();
	for (int i = 0; i < elements.size(); i++) {
		Element *it = elements.at(i);
		QGraphicsSimpleTextItem *elementItem = new QGraphicsSimpleTextItem(_elementPanel);
		elementItem->setBrush(whiteBrush);
		elementItem->setFont(font);
		elementItem->setPos(position);
		elementItem->setText(it->getName());

		position.setY(position.y() + 20);
		_elementLabels.append(elementItem);
	}

	// Fill in the attributes
	_attributeLabel->setText("Attributes:");
	position.setX(_attributeLabel->pos().x());
	position.setY(_attributeLabel->pos().y() + 40);
	QVector<QPair<Attribute*, int>> attributes = equipment->getAttributes();
	for (int i = 0; i < attributes.size(); i++) {
		QPair<Attribute*, int> it = attributes.at(i);
		QGraphicsSimpleTextItem *attributeItem = new QGraphicsSimpleTextItem(_attributePanel);
		attributeItem->setBrush(whiteBrush);
		attributeItem->setFont(font);
		attributeItem->setPos(position);
		attributeItem->setText(it.first->getName() + " +" + QString::number(it.second));

		position.setY(position.y() + 20);
		_attributeLabels.append(attributeItem);
	}
}