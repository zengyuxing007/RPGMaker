#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include "qevent.h"
#include "qvector.h"
#include "qgraphicsscene.h"

#include "Party.h"
#include "SoundItem.h"
#include "BattleItem.h"
#include "BattleAction.h"
#include "BattleMessage.h"
#include "BattleEnemyItem.h"
#include "BattleTargetPointer.h"
#include "BattleCharacterItem.h"
#include "BattleBackgroundItem.h"
#include "BattleCharacterStatus.h"
#include "BattleTargetSelection.h"
#include "BattleAbilitySelection.h"
#include "../AIModule/Headers/BattleAI.h"
#include "../Components/Headers/Battlegroup.h"
#include "../Components/Headers/AttributeSystem.h"

using aimodule::BattleAI;
using projectcomponents::Battlegroup;
using projectcomponents::AttributeSystem;

namespace playmodule {

	class BattleScene : public QGraphicsScene {

		Q_OBJECT

	public:

		enum Result {WIN, LOSE, FLEE};

		BattleScene(QObject *parent, SoundItem *sound);
		virtual ~BattleScene();

		void initialize(const QPixmap &battleback, Battlegroup *group, Party *party);
		void finalize();

		void setCharacters(Party *party);
		void setEnemies(Battlegroup *enemies);

	protected:
		virtual void keyPressEvent(QKeyEvent *event);

	private:
		void fillQueue();
		void startTurn();
		void characterTurn();
		void enemyTurn();
		void selectTarget();
		void actionEvaluation();
		void turnResults();
		void endBattle();

		void characterProgression();
		void distributeLoot();

		void finishBattleScene();

		void executeAttack();
		void executeAbility();
		void executeOffensiveAbility();
		void executeDefensiveAbility();

		// Panels
		BattleMessage *_messageBoard;
		BattleBackgroundItem *_background;
		BattleTargetPointer *_targetPointer;
		BattleCharacterStatus *_characterStatus;
		BattleTargetSelection *_targetSelection;
		BattleAbilitySelection *_abilitySelection;

		// Characters and enemies
		Party *_party;
		Battlegroup *_group;
		QVector<BattleEnemyItem*> _enemies;
		QVector<BattleEnemyItem*> _activeEnemies;
		QVector<BattleCharacterItem*> _characters;
		QVector<BattleCharacterItem*> _activeCharacters;
		QVector<BattleItem*> _battlersQueue;
		
		// Experience and Loot
		int _experience;
		int _currency;
		QVector<QPair<EquipmentPiece*, int>> _loot;

		// State variables
		void (BattleScene::*_nextStep) ();
		BattleAction *_turn;

		// Sound and AI
		SoundItem *_sound;
		BattleAI *_battleAI;

	private slots:
		void actionSelected(BattleAction::Action action);
		void targetSelected(BattleItem *target);

	signals:
		void battleFinished(BattleScene::Result result);
	};
}

#endif