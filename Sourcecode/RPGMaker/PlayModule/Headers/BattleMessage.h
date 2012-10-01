#ifndef BATTLEMESSAGE_H
#define BATTLEMESSAGE_H

#include "qobject.h"
#include "qgraphicsitem.h"

namespace playmodule {

	class BattleMessage : public QObject, public QGraphicsPixmapItem {

		Q_OBJECT

	public:
		BattleMessage(QGraphicsItem *parent);
		virtual ~BattleMessage();

		void setMessage(QString message);

	private:
		void createWindow();

		QGraphicsTextItem *_textField;
	};
}

#endif