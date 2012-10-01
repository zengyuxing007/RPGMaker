#include "qrect.h"
#include "qpoint.h"
#include "qpainter.h"

#include "../ProgramDefaults.h"
#include "../Headers/MenuMainCharacterItem.h"

using namespace playmodule;

MenuMainCharacterItem::MenuMainCharacterItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _currentFrame(0), _character(0) {

		QPixmap background(500, 100);
		background.fill(Qt::transparent);
		setPixmap(background);

		_characterModel = new QGraphicsPixmapItem(this);
		_characterModel->setPixmap(QPixmap(75, 75));
		_characterModel->setPos(12, 12);

		_selectionBorder = new QGraphicsPixmapItem(this);
		_selectionBorder->setPos(0, 0);
		_selectionBorder->hide();
		drawBorder();

		QFont font ("Times", 12, QFont::Bold);
		QBrush brush(Qt::white);

		_nameItem = new QGraphicsSimpleTextItem(this);
		_nameItem->setPos(100, 12);
		_nameItem->setBrush(brush);
		_nameItem->setFont(font);

		_levelItem = new QGraphicsSimpleTextItem(this);
		_levelItem->setPos(100, 32);
		_levelItem->setBrush(brush);
		_levelItem->setFont(font);

		_healthItem = new QGraphicsSimpleTextItem(this);
		_healthItem->setPos(100, 52);
		_healthItem->setBrush(brush);
		_healthItem->setFont(font);

		_energyItem = new QGraphicsSimpleTextItem(this);
		_energyItem->setPos(100, 72);
		_energyItem->setBrush(brush);
		_energyItem->setFont(font);
}

MenuMainCharacterItem::~MenuMainCharacterItem() {}

void MenuMainCharacterItem::initialize(BattleCharacterItem *character) {

	// Set character
	_character = character;

	// Set name and level
	_characterFrames = _character->getCharacter()->getEnvironmentImage().first;
	_nameItem->setText(QString("Name: ") + _character->getCharacter()->getName());
	_levelItem->setText(QString("Level: ") + QString::number(_character->getLevel()));
	
	// Set health
	QString healthText;
	healthText += _character->getMaxHealth().first->getName();
	healthText += ": ";
	healthText += QString::number(_character->getMaxHealth().second);
	_healthItem->setText(healthText);

	// Set energy
	QString energyText;
	energyText += _character->getMaxEnergy().first->getName();
	energyText += ": ";
	energyText += QString::number(_character->getMaxEnergy().second);
	_energyItem->setText(energyText);

	// Set character frame
	if (_character->getCharacter()->getEnvironmentImage().second != QString()) {
		QPoint frameIndex(_currentFrame % 4, _currentFrame / 4);
		QRect dimensions(frameIndex.x() * _characterFrames.width() / 4, frameIndex.y() * _characterFrames.height() / 4, _characterFrames.width() / 4, _characterFrames.height() / 4);
		QPixmap characterFrame = _characterFrames.copy(dimensions);
		
		if ((characterFrame.width() > 75) || (characterFrame.height() > 75))
			characterFrame = characterFrame.scaled(QSize(75, 75), Qt::KeepAspectRatio);

		QPixmap frame(75, 75);
		frame.fill(Qt::transparent);
		QPainter painter(&frame);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

		QPoint position(0, 0);
		position.setX(frame.width()/2 - characterFrame.width()/2);
		position.setY(frame.height()/2 - characterFrame.height()/2);
		painter.drawPixmap(position, characterFrame);

		painter.end();
		_characterModel->setPixmap(frame);
	}
}

void MenuMainCharacterItem::finalize() {
	_character = 0;
	_characterFrames = QPixmap();
	_characterModel->setPixmap(QPixmap(75, 75));
	_nameItem->setText(QString());
	_levelItem->setText(QString());
	_healthItem->setText(QString());
	_energyItem->setText(QString());

	_selectionBorder->hide();
}

void MenuMainCharacterItem::markCharacter() {
	_selectionBorder->show();
}

void MenuMainCharacterItem::unmarkCharacter() {
	_selectionBorder->hide();
}

void MenuMainCharacterItem::nextCharacterFrame() {
	_currentFrame = (_currentFrame + 1) % 16;
	QPoint frameIndex(_currentFrame % 4, _currentFrame / 4);
	QRect dimensions(frameIndex.x() * _characterFrames.width() / 4, frameIndex.y() * _characterFrames.height() / 4, _characterFrames.width() / 4, _characterFrames.height() / 4);
	QPixmap characterFrame = _characterFrames.copy(dimensions);

	if ((characterFrame.width() > 75) || (characterFrame.height() > 75))
		characterFrame = characterFrame.scaled(QSize(75, 75), Qt::KeepAspectRatio);

	QPixmap frame(75, 75);
	frame.fill(Qt::transparent);
	QPainter painter(&frame);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	QPoint position(0, 0);
	position.setX(frame.width()/2 - characterFrame.width()/2);
	position.setY(frame.height()/2 - characterFrame.height()/2);
	painter.drawPixmap(position, characterFrame);

	painter.end();
	_characterModel->setPixmap(frame);
}

BattleCharacterItem* MenuMainCharacterItem::getCharacter() const {
	return _character;
}

void MenuMainCharacterItem::drawBorder() {
	// Get and resize the textballoon
	QPixmap windowSkin(QPixmap(g_windowskinPath + "001-Blue01.png"));
	QPixmap panel(boundingRect().width(), boundingRect().height());
	panel.fill(Qt::transparent);

	// Paint the textballoon's border
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

	_selectionBorder->setPixmap(panel);
}