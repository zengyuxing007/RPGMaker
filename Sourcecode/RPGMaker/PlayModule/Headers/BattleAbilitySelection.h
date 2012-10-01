#ifndef BATTLEABILITYSELECTION_H
#define BATTLEABILITYSELECTION_H

#include "qevent.h"
#include "qobject.h"
#include "qvector.h"
#include "qgraphicsitem.h"

#include "BattleAction.h"
#include "BattleCharacterItem.h"

namespace playmodule {

	class BattleAbilitySelection : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:

		enum State {ACTIONSELECTION, ABILITYSELECTION};

		BattleAbilitySelection(QGraphicsItem *parent);
		virtual ~BattleAbilitySelection();

		BattleAction::Action getAction() const;
		Ability* getAbility() const;
		void setBattler(BattleItem *battler);

		void keyPressEvent(QKeyEvent *event);

	private:
		void createWindow();
		void createActions();
		void createAbilities();
		
		void nextItem();
		void previousItem();
		void itemSelected();
		void setCurrentItem(QGraphicsTextItem *item);

		State _state;
		BattleAction::Action _action;
		BattleItem *_currentBattler;
		QVector<Ability*> _abilities;

		int _abilityIndex;
		QGraphicsTextItem *_messageItem;
		QGraphicsTextItem *_currentItem;
		QVector<QGraphicsTextItem*> _items;
		QVector<QGraphicsTextItem*> _activeItems;

	signals:
		void actionSelected(BattleAction::Action action);
	};
}

#endif