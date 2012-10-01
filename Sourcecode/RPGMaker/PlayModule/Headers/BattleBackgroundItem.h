#ifndef BATTLEBACKGROUNDITEM_H
#define BATTLEBACKGROUNDITEM_H

#include "qgraphicsitem.h"

namespace playmodule {

	class BattleBackgroundItem : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		BattleBackgroundItem(QGraphicsItem *parent);
		virtual ~BattleBackgroundItem();

		void setBackground(const QPixmap &background);

	private:

	};
}
#endif