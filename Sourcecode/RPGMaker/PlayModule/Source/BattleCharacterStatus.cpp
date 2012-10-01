#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/BattleCharacterStatus.h"

using namespace playmodule;

BattleCharacterStatus::BattleCharacterStatus(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _characters(0), _info(0) {

		createWindow();
}

BattleCharacterStatus::~BattleCharacterStatus() {}

void BattleCharacterStatus::setCharacters(const QVector<BattleCharacterItem*> &characters) {
	for (int i = 0; i < _info.size(); i++) {
		delete _info.at(i);
	}
	_info.clear();

	_characters = characters;
	int y = 10;
	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *character = _characters.at(i);
		connect (character, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));

		QString infoString;
		infoString += character->getCharacter()->getName() + QString(":\n");
		infoString += QString("Health: ") + QString::number(character->getHealth().second) + QString(" / ") + QString::number(character->getMaxHealth().second) + QString("\t");
		infoString += QString("Energy: ") + QString::number(character->getEnergy().second) + QString(" / ") + QString::number(character->getMaxEnergy().second);

		QGraphicsTextItem *infoItem = new QGraphicsTextItem(this);
		_info.append(infoItem);
		infoItem->setTextWidth(boundingRect().width());
		infoItem->setDefaultTextColor(Qt::green);

		infoItem->setPlainText(infoString);
		QFont font("Times", 12, QFont::Bold);
		infoItem->setFont(font);

		QTextBlockFormat format;
		format.setAlignment(Qt::AlignLeft);
		QTextCursor cursor = infoItem->textCursor();
		cursor.select(QTextCursor::Document);
		cursor.mergeBlockFormat(format);
		cursor.clearSelection();
		infoItem->setTextCursor(cursor);

		infoItem->setPos(10, y);
		y += 50;
	}
}

void BattleCharacterStatus::createWindow() {
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

void BattleCharacterStatus::valueChanged(QString name) {
	BattleCharacterItem *character = 0;
	QGraphicsTextItem *infoItem = 0;
	for (int i = 0; i < _characters.size(); i++) {
		BattleCharacterItem *it = _characters.at(i);
		if (!!it && (it->getName() == name)) {
			character = it;
			infoItem = _info.at(i);
			break;
		}
	}

	if (!!character) {
		QString infoString;
		infoString += character->getCharacter()->getName() + QString(":\n");
		infoString += QString("Health: ") + QString::number(character->getHealth().second) + QString(" / ") + QString::number(character->getMaxHealth().second) + QString("\t");
		infoString += QString("Energy: ") + QString::number(character->getEnergy().second) + QString(" / ") + QString::number(character->getMaxEnergy().second);


		double percentage = (double)character->getHealth().second / (double)character->getMaxHealth().second * 100;

		if (percentage > 60)
			infoItem->setDefaultTextColor(Qt::green);
		else if ((percentage <= 60) && (percentage > 25))
			infoItem->setDefaultTextColor(Qt::yellow);
		else if ((percentage <= 25) && (percentage > 0))
			infoItem->setDefaultTextColor(Qt::red);
		else
			infoItem->setDefaultTextColor(Qt::gray);

		infoItem->setPlainText(infoString);
		QFont font("Times", 12, QFont::Bold);
		infoItem->setFont(font);

		QTextBlockFormat format;
		format.setAlignment(Qt::AlignLeft);
		QTextCursor cursor = infoItem->textCursor();
		cursor.select(QTextCursor::Document);
		cursor.mergeBlockFormat(format);
		cursor.clearSelection();
		infoItem->setTextCursor(cursor);
	}
}