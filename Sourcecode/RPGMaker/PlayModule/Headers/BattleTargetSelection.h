#ifndef BATTLETARGETSELECTION_H
#define BATTLETARGETSELECTION_H

#include "qevent.h"
#include "qobject.h"
#include "qgraphicsitem.h"

#include "BattleItem.h"
#include "BattleEnemyItem.h"
#include "BattleCharacterItem.h"
#include "BattleTargetPointer.h"

namespace playmodule {

	class BattleTargetSelection : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		BattleTargetSelection(QGraphicsItem *parent, BattleTargetPointer *pointer);
		virtual ~BattleTargetSelection();

		BattleItem *getTarget() const;
		void setTarget(BattleItem *item);
		void setBattlers(QVector<BattleCharacterItem*> characters, QVector<BattleEnemyItem*> enemies);

		void keyPressEvent(QKeyEvent *event);

	private:
		void createWindow();
		void nextItem();
		void previousItem();
		void itemSelected();
		void setCurrentItem(QGraphicsSimpleTextItem *item);

		int _currentTarget;
		BattleTargetPointer *_pointer;
		QVector<BattleItem*> _battlers;

		QGraphicsSimpleTextItem *_messageItem;
		QGraphicsSimpleTextItem *_currentItem;
		QVector<QGraphicsSimpleTextItem*> _items;
		QVector<QGraphicsSimpleTextItem*> _activeItems;

	signals:
		void targetSelected(BattleItem *target);
	};
}

#endif