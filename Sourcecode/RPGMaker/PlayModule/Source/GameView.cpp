#include "qscrollbar.h"

#include "../Headers/GameView.h"

using namespace playmodule;

GameView::GameView(QWidget *parent, Project *project)
	: QGraphicsView(parent),
	_project(project),
	_soundItem(new SoundItem(this)),
	_mainMenu(new MenuScene(this, _soundItem)),
	_battleScene(new BattleScene(this, _soundItem)),
	_environmentScene(new EnvironmentScene(this, _soundItem)) {

		horizontalScrollBar()->hide();
		verticalScrollBar()->hide();

		setScene(_environmentScene);

		connect(_environmentScene, SIGNAL(focusSignal()), this, SLOT(environmentFocus()));
		connect(_environmentScene, SIGNAL(openMenu()), this, SLOT(openMenu()));
		connect(_environmentScene, SIGNAL(startBattle(const QPixmap&, Battlegroup*)), this, SLOT(startBattle(const QPixmap&, Battlegroup*)));
		connect(_battleScene, SIGNAL(battleFinished(BattleScene::Result)), this, SLOT(endBattle(BattleScene::Result)));
		connect(_mainMenu, SIGNAL(closeMenu()), this, SLOT(closeMenu()));
}

GameView::~GameView() {}

void GameView::show() {
	environmentFocus();
	QGraphicsView::show();
}

void GameView::stop() {
	_soundItem->stop();
	_battleScene->finalize();
	_environmentScene->finalize();
}

BattleScene* GameView::getBattleScene() const {
	return _battleScene;
}

EnvironmentScene* GameView::getEnvironmentScene() const {
	return _environmentScene;
}

void GameView::setParty(Party *party) {
	_party = party;
}

void GameView::environmentFocus() {
	centerOn(_environmentScene->getFocusItem());
	_environmentScene->setFocus();
}

void GameView::openMenu() {
	_mainMenu->initialize(_party);
	setScene(_mainMenu);
}

void GameView::closeMenu() {
	_mainMenu->finalize();
	setScene(_environmentScene);
}

void GameView::startBattle(const QPixmap &background, Battlegroup *group) {
	_battleScene->initialize(background, group, _party);
	setScene(_battleScene);
}

void GameView::endBattle(BattleScene::Result result) {
	_battleScene->finalize();
	_environmentScene->currentEventFinished();
	setScene(_environmentScene);
}