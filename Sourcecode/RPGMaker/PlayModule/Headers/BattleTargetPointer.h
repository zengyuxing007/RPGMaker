#ifndef BATTLETARGETPOINTER_H
#define BATTLETARGETPOINTER_H

#include "qobject.h"
#include "qgraphicsitem.h"

#include "BattleItem.h"

namespace playmodule {

	class BattleTargetPointer : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:

		BattleTargetPointer(QGraphicsItem *parent);
		virtual ~BattleTargetPointer();

		BattleItem* getTarget() const;
		void setTarget(BattleItem *target);

	private:
		BattleItem *_target;

		QPixmap _friendlyPointer;
		QPixmap _hostilePointer;
	};
}

#endif