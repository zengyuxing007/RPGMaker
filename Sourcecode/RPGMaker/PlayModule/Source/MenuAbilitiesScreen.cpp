#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/MenuAbilitiesScreen.h"

using namespace playmodule;

MenuAbilitiesScreen::MenuAbilitiesScreen(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _currentAbility(0), _currentAbilityLabel(0) {

		createWindow();
		createLabels();
}

MenuAbilitiesScreen::~MenuAbilitiesScreen() {}

void MenuAbilitiesScreen::initialize(BattleCharacterItem *character) {
	QFont font("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	// Set character
	_character = character;

	// Build ability list
	QPointF position(_abilitiesLabel->pos().x(), _abilitiesLabel->pos().y() + 40);
	_abilitiesLabel->setText("Abilities of " + _character->getName() + ":");

	QVector<Ability*> abilities = _character->getAbilities();
	for (int i = 0; i < qMin<int>(abilities.size(), 25); i++) {
		Ability *it = abilities.at(i);
		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(_abilitiesPanel);
		item->setText(it->getName());
		item->setBrush(brush);
		item->setFont(font);
		item->setPos(position);

		_activeAbilityLabels.append(item);
		position.setY(position.y() + 20);
	}

	// Set current ability
	if (!abilities.empty()) {
		_currentAbility = 0;
		setCurrentAbility(_activeAbilityLabels.first());
	} else {
		_currentAbility = -1;
	}
}

void MenuAbilitiesScreen::finalize() {
	_character = 0;

	_currentAbility = 0;
	_abilitiesLabel->setText(QString());
	_currentAbilityLabel = 0;
	for (int i = 0; i < _activeAbilityLabels.size(); i++)
		delete _activeAbilityLabels.at(i);
	_activeAbilityLabels.clear();

	_nameLabel->setText(QString());
	_costLabel->setText(QString());
	_typeLabel->setText(QString());
	_attributeLabel->setText(QString());
	_descriptionLabel->setText(QString());
	_description->setPlainText(QString());

	_elementLabel->setText(QString());
	for (int i = 0; i < _elementLabels.size(); i++)
		delete _elementLabels.at(i);
	_elementLabels.clear();
}

void MenuAbilitiesScreen::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Down:
		nextAbility();
		break;
	case Qt::Key_Up:
		previousAbility();
		break;
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		emit closeAbilitiesScreen();
		break;
	}
}

void MenuAbilitiesScreen::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MenuAbilitiesScreen::createWindow() {
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

	_abilitiesPanel = new QGraphicsPixmapItem(this);
	_abilitiesPanel->setPixmap(panel.scaled(200, 600));
	_abilitiesPanel->setPos(0, 0);

	_descriptionPanel = new QGraphicsPixmapItem(this);
	_descriptionPanel->setPixmap(panel.scaled(600, 400));
	_descriptionPanel->setPos(_abilitiesPanel->boundingRect().width(), 0);

	_elementPanel = new QGraphicsPixmapItem(this);
	_elementPanel->setPixmap(panel.scaled(300, 400));
	_elementPanel->setPos(_descriptionPanel->pos().x(), _descriptionPanel->pos().y() + _descriptionPanel->boundingRect().height());
}

void MenuAbilitiesScreen::createLabels() {
	QFont font("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	// Abilities
	_abilitiesLabel = new QGraphicsSimpleTextItem(_abilitiesPanel);
	_abilitiesLabel->setBrush(brush);
	_abilitiesLabel->setFont(font);
	_abilitiesLabel->setPos(25, 10);

	// Description
	_nameLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_nameLabel->setBrush(brush);
	_nameLabel->setFont(font);
	_nameLabel->setPos(25, 10);

	_costLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_costLabel->setBrush(brush);
	_costLabel->setFont(font);
	_costLabel->setPos(25, 30);

	_typeLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_typeLabel->setBrush(brush);
	_typeLabel->setFont(font);
	_typeLabel->setPos(25, 50);

	_attributeLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_attributeLabel->setBrush(brush);
	_attributeLabel->setFont(font);
	_attributeLabel->setPos(25, 70);

	_descriptionLabel = new QGraphicsSimpleTextItem(_descriptionPanel);
	_descriptionLabel->setBrush(brush);
	_descriptionLabel->setFont(font);
	_descriptionLabel->setPos(25, 90);

	_description = new QGraphicsTextItem(_descriptionPanel);
	_description->setTextWidth(_descriptionPanel->boundingRect().width() - 25);
	_description->setPos(20, 110);

	// Elements
	_elementLabel = new QGraphicsSimpleTextItem(_elementPanel);
	_elementLabel->setBrush(brush);
	_elementLabel->setFont(font);
	_elementLabel->setPos(25, 10);
}

void MenuAbilitiesScreen::nextAbility() {
	if (_currentAbility < 0)
		return;

	if (_currentAbilityLabel == _activeAbilityLabels.last()) {

		// If the last ability was selected, scroll back to the first one
		if (_currentAbility == (_character->getAbilities().size()-1)) {
			for (int i = 0; i < _activeAbilityLabels.size(); i++) {
				QGraphicsSimpleTextItem *item = _activeAbilityLabels.at(i);
				Ability *ability = _character->getAbilities().at(i);
				item->setText(ability->getName());
			}

			_currentAbility = 0;
			setCurrentAbility(_activeAbilityLabels.at(0));

		} else { // Else, scroll to the next ability
			for (int i = 0; i < _activeAbilityLabels.size() -1; i++) {
				QGraphicsSimpleTextItem *item = _activeAbilityLabels.at(i);
				item->setText(_activeAbilityLabels.at(i+1)->text());
			}

			_currentAbility++;
			Ability *ability = _character->getAbilities().at(_currentAbility);

			QGraphicsSimpleTextItem *item = _activeAbilityLabels.last();
			item->setText(ability->getName());
			setCurrentAbility(item);
		}

	} else {
		_currentAbility++;
		setCurrentAbility(_activeAbilityLabels.at(_activeAbilityLabels.indexOf(_currentAbilityLabel)+1));
	}
}

void MenuAbilitiesScreen::previousAbility() {
	if (_currentAbility < 0)
		return;

	if (_currentAbilityLabel == _activeAbilityLabels.first()) {

		// If the first ability was selected, scroll to the last one
		if (_currentAbility == 0) {
			for (int i = (_activeAbilityLabels.size()-1); i >= 0; i--) {
				QGraphicsSimpleTextItem *item = _activeAbilityLabels.at(i);
				Ability *ability = _character->getAbilities().at(_character->getAbilities().size() - _activeAbilityLabels.size() + i);
				item->setText(ability->getName());
			}

			_currentAbility = _character->getAbilities().size() -1;
			setCurrentAbility(_activeAbilityLabels.last());

		} else { // Else scroll to the previous ability

			for (int i = (_activeAbilityLabels.size() -1); i > 0; i--) {
				QGraphicsSimpleTextItem *item = _activeAbilityLabels.at(i);
				item->setText(_activeAbilityLabels.at(i-1)->text());
			}

			_currentAbility--;
			Ability *ability = _character->getAbilities().at(_currentAbility);

			QGraphicsSimpleTextItem *item = _activeAbilityLabels.first();
			item->setText(ability->getName());
			setCurrentAbility(_currentAbilityLabel);
		}

	} else {
		_currentAbility--;
		setCurrentAbility(_activeAbilityLabels.at(_activeAbilityLabels.indexOf(_currentAbilityLabel) -1));
	}
}

void MenuAbilitiesScreen::setCurrentAbility(QGraphicsSimpleTextItem *item) {
	QBrush whiteBrush(Qt::white);
	QBrush yellowBrush(Qt::yellow);
	QFont font("Times", 12, QFont::Bold);

	// Set current selected item
	if (!!_currentAbilityLabel)
		_currentAbilityLabel->setBrush(whiteBrush);

	_currentAbilityLabel = item;
	_currentAbilityLabel->setBrush(yellowBrush);

	// Get corresponding ability
	Ability *ability = _character->getAbilities().at(_currentAbility);

	// Clean the panels
	_elementLabel->setText(QString());
	for (int i = 0; i < _elementLabels.size(); i++)
		delete _elementLabels.at(i);
	_elementLabels.clear();

	// Fill in the description
	_nameLabel->setText("Name: " + ability->getName());
	_costLabel->setText("Cost: " + QString::number(ability->getCost()) + " " + _character->getEnergy().first->getName());
	_attributeLabel->setText("Attribute: " + ability->getAttribute().first->getName() + " x" + QString::number(ability->getAttribute().second));
	switch (ability->getType()) {
	case Ability::OFFENSIVE:
		_typeLabel->setText("Type: Offensive");
		break;
	case Ability::DEFENSIVE:
		_typeLabel->setText("Type: Defensive:");
		break;
	}

	_descriptionLabel->setText("Description:");
	_description->setPlainText(ability->getDescription());
	_description->setFont(font);
	_description->setDefaultTextColor(Qt::white);

	QTextBlockFormat format;
	format.setAlignment(Qt::AlignLeft);
	QTextCursor cursor = _description->textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(format);
	cursor.clearSelection();
	_description->setTextCursor(cursor);

	// Fill in the Elements
	_elementLabel->setText("Elements:");
	QPointF position(_elementLabel->pos().x(), _elementLabel->pos().y() + 40);
	QVector<Element*> elements = ability->getElements();
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
}