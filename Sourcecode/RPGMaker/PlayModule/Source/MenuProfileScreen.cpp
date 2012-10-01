#include "qpainter.h"
#include "qtextbrowser.h"

#include "../ProgramDefaults.h"
#include "../Headers/MenuProfileScreen.h"

using namespace playmodule;

MenuProfileScreen::MenuProfileScreen(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent), _timer(new QTimer(this)), _character(0), _currentFrame(0), _offensiveItems(0), _defensiveItems(0) {

		createWindow();
		createImagePanel();
		createProfilePanel();
		createGeneralAttributesPanel();
		createOffensiveAttributesPanel();
		createDefensiveAttributesPanel();

		_timer->setInterval(250);
		_timer->setSingleShot(true);
		connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

MenuProfileScreen::~MenuProfileScreen() {}

void MenuProfileScreen::initialize(BattleCharacterItem *character) {
	QFont font("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	// Set character
	_character = character;

	// Set name, level and biography
	_environmentalFrames = _character->getCharacter()->getEnvironmentImage().first;
	_nameItem->setText(QString("Name: ") + _character->getCharacter()->getName());
	_levelItem->setText(QString("Level: ") + QString::number(_character->getLevel()));
	_experienceItem->setText(QString("Experience: " + QString::number(_character->getExperience()) + " / " + QString::number(_character->getRequiredExperience())));
	_classItem->setText(QString("Class: ") + _character->getClass()->getName());

	_biographyItem->setPlainText(QString("Biography:\n") + _character->getCharacter()->getBiography());
	_biographyItem->setFont(font);

	QTextBlockFormat format;
	format.setAlignment(Qt::AlignLeft);
	QTextCursor cursor = _biographyItem->textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(format);
	cursor.clearSelection();
	_biographyItem->setTextCursor(cursor);

	// Set environmental image
	if (_character->getCharacter()->getEnvironmentImage().second != QString()) {
		QPoint frameIndex(_currentFrame % 4, _currentFrame / 4);
		QRect dimensions(frameIndex.x() * _environmentalFrames.width() / 4, frameIndex.y() * _environmentalFrames.height() / 4, _environmentalFrames.width() / 4, _environmentalFrames.height() / 4);
		QPixmap characterFrame = _environmentalFrames.copy(dimensions);

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
		_environmentalModel->setPixmap(frame);
	}

	// Set battler image
	if (_character->getCharacter()->getBattleImage().second != QString()) {
		QPixmap battlerImage = _character->getCharacter()->getBattleImage().first;

		if ((battlerImage.width() > _battleModel->boundingRect().width()) || (battlerImage.height() > _battleModel->boundingRect().height()))
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

		_battleModel->setPixmap(frame);
	}

	// Set general attributes
	_generalItem->setText("Standard Attributes:");
	_healthItem->setText(_character->getHealth().first->getName() + ": " + QString::number(_character->getMaxHealth().second / g_healthMultiplier));
	_energyItem->setText(_character->getEnergy().first->getName() + ": " + QString::number(_character->getMaxEnergy().second / g_energyMultiplier));
	_agilityItem->setText(_character->getAgility().first->getName() + ": " + QString::number(_character->getAgility().second));
	_evasionItem->setText(_character->getEvasion().first->getName() + ": " + QString::number(_character->getEvasion().second));

	// Set offensive attributes
	_offensiveItem->setText("Offensive Attributes:");

	QPointF position(_offensiveItem->pos().x(), _offensiveItem->pos().y()+20);
	QVector<QPair<Attribute*, int>> offensiveAttrs = _character->getOffensiveAttributes();
	for (int i = 0; i < offensiveAttrs.size(); i++) {
		QPair<Attribute*, int> attribute = offensiveAttrs.at(i);
		QGraphicsSimpleTextItem *attributeItem = new QGraphicsSimpleTextItem(_offensiveAttributesPanel);
		attributeItem->setBrush(brush);
		attributeItem->setFont(font);
		attributeItem->setPos(position);
		attributeItem->setText(attribute.first->getName() + ": " + QString::number(attribute.second));

		position.setY(position.y()+20);
		_offensiveItems.append(attributeItem);
	}

	
	// Set defensive attributes
	_defensiveItem->setText("Defensive Attributes:");

	position.setX(_defensiveItem->pos().x());
	position.setY(_defensiveItem->pos().y()+20);
	QVector<QPair<Attribute*, int>> defensiveAttrs = _character->getDefensiveAttributes();
	for (int i = 0; i < defensiveAttrs.size(); i++) {
		QPair<Attribute*, int> attribute = defensiveAttrs.at(i);
		QGraphicsSimpleTextItem *attributeItem = new QGraphicsSimpleTextItem(_defensiveAttributesPanel);
		attributeItem->setBrush(brush);
		attributeItem->setFont(font);
		attributeItem->setPos(position);
		attributeItem->setText(attribute.first->getName() + ": " + QString::number(attribute.second));

		position.setY(position.y()+20);
		_defensiveItems.append(attributeItem);
	}

	_timer->start();
}

void MenuProfileScreen::finalize() {
	_character = 0;
	_timer->stop();

	_currentFrame = 0;
	_environmentalFrames = QPixmap();
	_battleModel->setPixmap(QPixmap(200, 200));
	_environmentalModel->setPixmap(QPixmap(75, 75));

	_nameItem->setText(QString());
	_levelItem->setText(QString());
	_experienceItem->setText(QString());
	_classItem->setText(QString());
	_biographyItem->setPlainText(QString());

	_generalItem->setText(QString());
	_healthItem->setText(QString());
	_energyItem->setText(QString());
	_agilityItem->setText(QString());
	_evasionItem->setText(QString());

	_offensiveItem->setText(QString());
	for (int i = 0; i < _offensiveItems.size(); i++)
		delete _offensiveItems.at(i);
	_offensiveItems.clear();

	_defensiveItem->setText(QString());
	for (int i = 0; i < _defensiveItems.size(); i++)
		delete _defensiveItems.at(i);
	_defensiveItems.clear();
}

BattleCharacterItem* MenuProfileScreen::getCharacter() const {
	return _character;
}

void MenuProfileScreen::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		emit closeCharacterProfile();
		break;
	}
}

void MenuProfileScreen::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MenuProfileScreen::createWindow() {
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

	_imagesPanel = new QGraphicsPixmapItem(this);
	_imagesPanel->setPixmap(panel.scaled(400, 300));
	_imagesPanel->setPos(400, 0);

	_generalInfoPanel = new QGraphicsPixmapItem(this);
	_generalInfoPanel->setPixmap(panel.scaled(400, 300));
	_generalInfoPanel->setPos(0, 0);

	_generalAttributesPanel = new QGraphicsPixmapItem(this);
	_generalAttributesPanel->setPixmap(panel.scaled(267, 300));
	_generalAttributesPanel->setPos(0, _generalInfoPanel->pos().y() + _generalInfoPanel->boundingRect().height());

	_offensiveAttributesPanel = new QGraphicsPixmapItem(this);
	_offensiveAttributesPanel->setPixmap(panel.scaled(266, 300));
	_offensiveAttributesPanel->setPos(_generalAttributesPanel->pos().x() + _generalAttributesPanel->boundingRect().width(), _generalAttributesPanel->pos().y());

	_defensiveAttributesPanel = new QGraphicsPixmapItem(this);
	_defensiveAttributesPanel->setPixmap(panel.scaled(267, 300));
	_defensiveAttributesPanel->setPos(_offensiveAttributesPanel->pos().x() + _offensiveAttributesPanel->boundingRect().width(), _offensiveAttributesPanel->pos().y());

}

void MenuProfileScreen::createImagePanel() {
	_battleModel = new QGraphicsPixmapItem(_imagesPanel);
	QPixmap battleModel(200, 200);
	battleModel.fill(Qt::transparent);
	_battleModel->setPixmap(battleModel);
	_battleModel->setPos(50, 50);

	_environmentalModel = new QGraphicsPixmapItem(_imagesPanel);
	QPixmap environmentalModel(75, 75);
	environmentalModel.fill(Qt::transparent);
	_environmentalModel->setPixmap(environmentalModel);
	_environmentalModel->setPos(_battleModel->pos().x() + _battleModel->boundingRect().width() + 50, 120);
}

void MenuProfileScreen::createProfilePanel() {
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	_nameItem = new QGraphicsSimpleTextItem(_generalInfoPanel);
	_nameItem->setPos(25, 10);
	_nameItem->setBrush(brush);
	_nameItem->setFont(font);

	_levelItem = new QGraphicsSimpleTextItem(_generalInfoPanel);
	_levelItem->setPos(25, 30);
	_levelItem->setBrush(brush);
	_levelItem->setFont(font);

	_experienceItem = new QGraphicsSimpleTextItem(_generalInfoPanel);
	_experienceItem->setPos(25, 50);
	_experienceItem->setBrush(brush);
	_experienceItem->setFont(font);

	_classItem = new QGraphicsSimpleTextItem(_generalInfoPanel);
	_classItem->setPos(25, 70);
	_classItem->setBrush(brush);
	_classItem->setFont(font);

	_biographyItem = new QGraphicsTextItem(_generalInfoPanel);
	_biographyItem->setPos(20, 90);
	_biographyItem->setTextWidth(370);
	_biographyItem->setDefaultTextColor(Qt::white);
}

void MenuProfileScreen::createGeneralAttributesPanel() {
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	_generalItem = new QGraphicsSimpleTextItem(_generalAttributesPanel);
	_generalItem->setPos(25, 10);
	_generalItem->setBrush(brush);
	_generalItem->setFont(font);

	_healthItem = new QGraphicsSimpleTextItem(_generalAttributesPanel);
	_healthItem->setPos(25, 30);
	_healthItem->setBrush(brush);
	_healthItem->setFont(font);

	_energyItem = new QGraphicsSimpleTextItem(_generalAttributesPanel);
	_energyItem->setPos(25, 50);
	_energyItem->setBrush(brush);
	_energyItem->setFont(font);

	_agilityItem = new QGraphicsSimpleTextItem(_generalAttributesPanel);
	_agilityItem->setPos(25, 70);
	_agilityItem->setBrush(brush);
	_agilityItem->setFont(font);

	_evasionItem = new QGraphicsSimpleTextItem(_generalAttributesPanel);
	_evasionItem->setPos(25, 90);
	_evasionItem->setBrush(brush);
	_evasionItem->setFont(font);
}

void MenuProfileScreen::createOffensiveAttributesPanel() {
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	_offensiveItem = new QGraphicsSimpleTextItem(_offensiveAttributesPanel);
	_offensiveItem->setPos(25, 10);
	_offensiveItem->setBrush(brush);
	_offensiveItem->setFont(font);
}

void MenuProfileScreen::createDefensiveAttributesPanel() {
	QFont font ("Times", 12, QFont::Bold);
	QBrush brush(Qt::white);

	_defensiveItem = new QGraphicsSimpleTextItem(_defensiveAttributesPanel);
	_defensiveItem->setPos(25, 10);
	_defensiveItem->setBrush(brush);
	_defensiveItem->setFont(font);
}

void MenuProfileScreen::timeout() {
	_currentFrame = (_currentFrame + 1) % 16;
	QPoint frameIndex(_currentFrame % 4, _currentFrame / 4);
	QRect dimensions(frameIndex.x() * _environmentalFrames.width() / 4, frameIndex.y() * _environmentalFrames.height() / 4, _environmentalFrames.width() / 4, _environmentalFrames.height() / 4);
	QPixmap characterFrame = _environmentalFrames.copy(dimensions);

	if ((characterFrame.width() > 75) || (characterFrame.height() > 75))
		_environmentalModel->setPixmap(characterFrame.scaled(QSize(75, 75), Qt::KeepAspectRatio));
	else {
		QPixmap frame(75, 75);
		frame.fill(Qt::transparent);
		QPainter painter(&frame);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

		QPoint position(0, 0);
		position.setX(frame.width()/2 - characterFrame.width()/2);
		position.setY(frame.height()/2 - characterFrame.height()/2);
		painter.drawPixmap(position, characterFrame);

		painter.end();
		_environmentalModel->setPixmap(frame);

	}

	_timer->start();
}