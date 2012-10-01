#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "qgraphicsview.h"

#include "Party.h"
#include "MenuScene.h"
#include "SoundItem.h"
#include "GameEvent.h"
#include "BattleScene.h"
#include "EnvironmentScene.h"
#include "../Components/Headers/Project.h"

namespace playmodule {

	class GameView: public QGraphicsView {

		Q_OBJECT

	public:
		GameView(QWidget *parent, Project *project);
		virtual ~GameView();

		void show();
		void stop();

		BattleScene* getBattleScene() const;
		EnvironmentScene* getEnvironmentScene() const;

		void setParty(Party *party);

	private:

		Party *_party;
		Project *_project;
		MenuScene *_mainMenu;
		SoundItem *_soundItem;
		BattleScene *_battleScene;
		QVector<GameEvent*> _events;
		EnvironmentScene *_environmentScene;

	private slots:
		void environmentFocus();
		void openMenu();
		void closeMenu();
		void startBattle(const QPixmap &background, Battlegroup *group);
		void endBattle(BattleScene::Result);
	};
}

#endif