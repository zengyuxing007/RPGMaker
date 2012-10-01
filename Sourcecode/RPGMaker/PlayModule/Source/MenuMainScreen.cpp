#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/MenuMainScreen.h"

using namespace playmodule;

MenuMainScreen::MenuMainScreen(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _state(OPTION_SELECTION), _options(0), _currentOption(0),
	_currentCharacterPanel(0), _characters(0), _characterPanels(0), _activeCharacterPanels(0), _timer(0) {
		createPanels();
		createCharacters();
		createTextfield();
		createOptions();

		_timer = new QTimer(this);
		_timer->setInterval(250);
		_timer->setSingleShot(true);
		connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

MenuMainScreen::~MenuMainScreen() {}

void MenuMainScreen::initialize(Party *party) {
	setCurrentCharacter(0);
	_characters = party->getPartyMembers();
	_currentCharacter = 0;

	for (int i = 0; i < qMin<int>(_characters.size(), _characterPanels.size()); i++) {
		MenuMainCharacterItem *panel = _characterPanels.at(i);
		BattleCharacterItem *character = _characters.at(i);

		panel->initialize(character);
		panel->show();
		_activeCharacterPanels.append(panel);
	}

	_timer->start();
}

void MenuMainScreen::finalize() {

	_state = OPTION_SELECTION;
	_currentOption = _options.first();

	_currentCharacterPanel = 0;
	_currentCharacter = 0;
	_characters.clear();
	_activeCharacterPanels.clear();

	for (int i = 0; i < _characterPanels.size(); i++) {
		_characterPanels.at(i)->finalize();
		_characterPanels.at(i)->hide();
	}

	_timer->stop();
}

void MenuMainScreen::keyPressEvent(QKeyEvent *event) {
	if (_state == OPTION_SELECTION) {
		switch (event->key()) {
		case Qt::Key_Down:
			nextOption();
			break;
		case Qt::Key_Up:
			previousOption();
			break;
		case Qt::Key_Return:
		case Qt::Key_Enter:
			optionSelected();
			break;
		case Qt::Key_Escape:
			emit closeMainMenu();
			break;
		}
	} else if (_state == CHARACTER_SELECTION) {
		switch (event->key()) {
		case Qt::Key_Down:
			nextCharacter();
			break;
		case Qt::Key_Up:
			previousCharacter();
			break;
		case Qt::Key_Return:
		case Qt::Key_Enter:
			characterSelected();
			break;
		case Qt::Key_Escape:
		case Qt::Key_Backspace:
			clearCharacterSelection();
			break;
		}
	}
}

void MenuMainScreen::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MenuMainScreen::createPanels() {
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

	// Party panel
	_partyPanel = new QGraphicsPixmapItem(this);
	_partyPanel->setPixmap(panel.scaled(QSize(600, 500)));
	_partyPanel->setPos(0, 0);
	
	// Options panel
	_optionsPanel = new QGraphicsPixmapItem(this);
	_optionsPanel->setPixmap(panel.scaled(QSize(200, 600)));
	_optionsPanel->setPos(600, 0);

	// Text panel
	_textPanel = new QGraphicsPixmapItem(this);
	_textPanel->setPixmap(panel.scaled(600, 100));
	_textPanel->setPos(0, 500);
}

void MenuMainScreen::createCharacters() {
	QPoint position(50, 50);
	for (int i = 0; i < 3; i++) {
		MenuMainCharacterItem *item = new MenuMainCharacterItem(this);
		item->setPos(position.x(), position.y());
		item->finalize();
		item->hide();
		position.setY(position.y() + item->boundingRect().height() + 25);
		_characterPanels.append(item);
	}
}

void MenuMainScreen::createOptions() {
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	QStringList options;
	options << "Character" << "Equipment" << "Abilities" << "Inventory" << "Exit";
	int x = 25, y = 40;
	for (int i = 0; i < options.size(); i++) {
		QGraphicsSimpleTextItem *item = new QGraphicsSimpleTextItem(options.at(i), _optionsPanel);
		item->setBrush(brush);
		item->setFont(font);
		item->setPos(x, y);
		_options.append(item);

		y += 30;
	}

	setCurrentOption(_options.at(0));
}

void MenuMainScreen::createTextfield() {
	_textField = new QGraphicsTextItem(_textPanel);
	_textField->setTextWidth(_textPanel->boundingRect().width()-50);
	_textField->setDefaultTextColor(QColor(Qt::white));
	_textField->setPos(25, 10);
}

void MenuMainScreen::nextOption() {
	if (_currentOption == _options.last()) {
		setCurrentOption(_options.first());
	} else {
		setCurrentOption(_options.at(_options.indexOf(_currentOption)+1));
	}
}

void MenuMainScreen::previousOption() {
	if (_currentOption == _options.first()) {
		setCurrentOption(_options.last());
	} else {
		setCurrentOption(_options.at(_options.indexOf(_currentOption)-1));
	}
}

void MenuMainScreen::nextCharacter() {
	if (_currentCharacterPanel == _activeCharacterPanels.at(_activeCharacterPanels.size()-1)) {

		// If the last character was selected, scroll back to the first one
		if (_currentCharacter == (_characters.size()-1)) {
			for (int i = 0; i < _activeCharacterPanels.size(); i++) {
				MenuMainCharacterItem *panel = _activeCharacterPanels.at(i);
				panel->finalize();
				panel->initialize(_characters.at(i));
			}

			_currentCharacter = 0;
			setCurrentCharacter(_characterPanels.at(0));

		} else { // Else scroll to the next character
			for (int i = 0; i < _activeCharacterPanels.size()-1; i++) {
				MenuMainCharacterItem *panel = _activeCharacterPanels.at(i);
				panel->finalize();
				panel->initialize(_activeCharacterPanels.at(i+1)->getCharacter());
			}

			_currentCharacter++;
			BattleCharacterItem *character = _characters.at(_currentCharacter);

			MenuMainCharacterItem *panel = _activeCharacterPanels.at(_activeCharacterPanels.size()-1);
			panel->finalize();
			panel->initialize(character);
			setCurrentCharacter(panel);
		}
	} else {
		_currentCharacter++;
		setCurrentCharacter(_activeCharacterPanels.at(_activeCharacterPanels.indexOf(_currentCharacterPanel)+1));
	}
}

void MenuMainScreen::previousCharacter() {
	if (_currentCharacterPanel == _activeCharacterPanels.at(0)) {

		// If the first character was selected, scroll to the last one
		if (_currentCharacter == 0) {
			for (int i = (_activeCharacterPanels.size()-1); i >= 0; i--) {
				MenuMainCharacterItem *panel = _activeCharacterPanels.at(i);
				panel->finalize();
				panel->initialize(_characters.at(_characters.size() - _activeCharacterPanels.size() + i));
			}

			_currentCharacter = _characters.size() -1;
			setCurrentCharacter(_activeCharacterPanels.at(_activeCharacterPanels.size()-1));

		} else { // Else scroll to the previous character
			for (int i = (_activeCharacterPanels.size()-1); i > 0; i--) {
				MenuMainCharacterItem *panel = _activeCharacterPanels.at(i);
				panel->finalize();
				panel->initialize(_activeCharacterPanels.at(i-1)->getCharacter());
			}

			_currentCharacter--;
			BattleCharacterItem *character = _characters.at(_currentCharacter);

			MenuMainCharacterItem *panel = _activeCharacterPanels.at(0);
			panel->finalize();
			panel->initialize(character);
			setCurrentCharacter(panel);
		}
	} else {
		_currentCharacter--;
		setCurrentCharacter(_activeCharacterPanels.at(_activeCharacterPanels.indexOf(_currentCharacterPanel)-1));
	}
}

void MenuMainScreen::optionSelected() {
	if (!!_currentOption) {
		if (_currentOption->text() == QString("Character")) {
			_state = MenuMainScreen::CHARACTER_SELECTION;
			_currentCharacter = 0;
			setCurrentCharacter(_activeCharacterPanels.at(0));
			return;
		} else if (_currentOption->text() == QString("Equipment")) {
			_state = MenuMainScreen::CHARACTER_SELECTION;
			_currentCharacter = 0;
			setCurrentCharacter(_activeCharacterPanels.at(0));
			return;
		} else if (_currentOption->text() == QString("Abilities")) {
			_state = MenuMainScreen::CHARACTER_SELECTION;
			_currentCharacter = 0;
			setCurrentCharacter(_activeCharacterPanels.at(0));
			return;
		} else if (_currentOption->text() == QString("Inventory")) {
			emit viewInventory();
			return;
		} else if (_currentOption->text() == QString("Exit")) {
			emit closeMainMenu();
			return;
		}

	}
}

void MenuMainScreen::characterSelected() {
	if (_options.indexOf(_currentOption) == 0)
		emit viewCharacterProfile(_currentCharacterPanel->getCharacter());
	else if (_options.indexOf(_currentOption) == 1)
		emit viewCharacterEquipment(_currentCharacterPanel->getCharacter());
	else if (_options.indexOf(_currentOption) == 2)
		emit viewCharacterAbilities(_currentCharacterPanel->getCharacter());
}

void MenuMainScreen::clearCharacterSelection() {
	_currentCharacter = 0;
	_currentCharacterPanel = 0;

	for (int i = 0; i < _activeCharacterPanels.size(); i++) {
		MenuMainCharacterItem *panel = _activeCharacterPanels.at(i);
		panel->finalize();
		panel->initialize(_characters.at(i));
	}

	_state = MenuMainScreen::OPTION_SELECTION;
}

void MenuMainScreen::setCurrentOption(QGraphicsSimpleTextItem *option) {
	QBrush whiteBrush(Qt::white);
	QBrush yellowBrush(Qt::yellow);

	if (!!_currentOption)
		_currentOption->setBrush(whiteBrush);

	_currentOption = option;
	_currentOption->setBrush(yellowBrush);

	if (option->text() == QString("Character"))
		_textField->setPlainText("View more information about the selected character.");
	else if (option->text() == QString("Equipment"))
		_textField->setPlainText("View or change the equipment the selected character is wearing.");
	else if (option->text() == QString("Abilities"))
		_textField->setPlainText("View the abilities of the selected character.");
	else if (option->text() == QString("Inventory"))
		_textField->setPlainText("View the items collected by the party.");
	else if (option->text() == QString("Exit"))
		_textField->setPlainText("Exit the menu.");

	QFont font("Times", 12, QFont::Bold);
	_textField->setFont(font);

	QTextBlockFormat format;
	format.setAlignment(Qt::AlignLeft);
	QTextCursor cursor = _textField->textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(format);
	cursor.clearSelection();
	_textField->setTextCursor(cursor);
}

void MenuMainScreen::setCurrentCharacter(MenuMainCharacterItem *character) {
	if (!!_currentCharacterPanel)
		_currentCharacterPanel->unmarkCharacter();

	if (!!character)
		character->markCharacter();

	_currentCharacterPanel = character;
}

void MenuMainScreen::timeout() {
	for (int i = 0; i < _characterPanels.size(); i++)
		_characterPanels.at(i)->nextCharacterFrame();

	_timer->start();
}